#include "tensorrt_module.h"

TensorRT_module::TensorRT_module() {
    initializeSampleParams();
    initializeVariables();
    if (!build()) {
        std::cout << "TensorRT build failed" << std::endl;
    }
}

TensorRT_module::TensorRT_module(std::string Name_Network) {
    initializeSampleParams();
    initializeVariables();
    mParams.uffFileName = Name_Network;
    if (!build()) {
        std::cout << "TensorRT build failed" << std::endl;
    }
}

TensorRT_module::~TensorRT_module() {
    teardown();
    delete buffers;
}

//!
//! \brief Creates the network, configures the builder and creates the network engine
//!
//! \details This function creates the MNIST network by parsing the Uff model
//!          and builds the engine that will be used to run MNIST (mEngine)
//!
//! \return Returns true if the engine was created successfully and false otherwise
//!
bool TensorRT_module::build() {
    initLibNvInferPlugins(&sample::gLogger.getTRTLogger(), "");

    std::cout << "[TensorRT] Build tensorRT model" << std::endl;
    auto builder = SampleUniquePtr<nvinfer1::IBuilder>(nvinfer1::createInferBuilder(sample::gLogger.getTRTLogger()));
    if (!builder) {
        return false;
    }

    auto network = SampleUniquePtr<nvinfer1::INetworkDefinition>(builder->createNetwork());
    if (!network) {
        return false;
    }

    auto config = SampleUniquePtr<nvinfer1::IBuilderConfig>(builder->createBuilderConfig());
    if (!config) {
        return false;
    }

    auto parser = SampleUniquePtr<nvuffparser::IUffParser>(nvuffparser::createUffParser());
    if (!parser) {
        return false;
    }

    auto constructed = constructNetwork(builder, network, config, parser);
    if (!constructed) {
        return false;
    }

    assert(network->getNbInputs() == 1);
    mInputDims = network->getInput(0)->getDimensions();
    assert(mInputDims.nbDims == 3);
    assert(network->getNbOutputs() == 1);
    return true;
}

//!
//! \brief Uses a Uff parser to create the MNIST Network and marks the output layers
//!
//! \param network Pointer to the network that will be populated with the MNIST network
//!
//! \param builder Pointer to the engine builder
//!
bool TensorRT_module::constructNetwork(SampleUniquePtr<nvinfer1::IBuilder>& builder,
                      SampleUniquePtr<nvinfer1::INetworkDefinition>& network,
                      SampleUniquePtr<nvinfer1::IBuilderConfig>& config,
                      SampleUniquePtr<nvuffparser::IUffParser>& parser) {
    // Register tensorflow input
    const int32_t imageH = IMAGE_H;
    const int32_t imageW = IMAGE_W;
    const int32_t imageC = IMAGE_C;

    parser->registerInput(
        mParams.inputTensorNames[0].c_str(), nvinfer1::Dims3(imageH, imageW, imageC), nvuffparser::UffInputOrder::kNHWC);

    // Register tensorflow output
    parser->registerOutput(mParams.outputTensorNames[0].c_str());

    auto parsed = parser->parse(locateFile(mParams.uffFileName, mParams.dataDirs).c_str(), *network, DataType::kFLOAT);
    if (!parsed) {
        return false;
    }

    builder->setMaxBatchSize(mParams.batchSize);
    config->setMaxWorkspaceSize(8_GiB);
    if (mParams.fp16) {
        // config->setFlag(BuilderFlag::kFP16);
        config->setFlag(BuilderFlag::kTF32);
    }
    samplesCommon::enableDLA(builder.get(), config.get(), mParams.dlaCore);

    // Calibrator life time needs to last until after the engine is built.
    std::unique_ptr<IInt8Calibrator> calibrator;

    if (mParams.int8) {
        sample::gLogInfo << "Using Entropy Calibrator 2" << std::endl;
        const std::string listFileName = "list.txt";
        nvinfer1::Dims4 imageDims{};
        imageDims = nvinfer1::Dims4{mParams.calBatchSize, imageH, imageW, imageC};
        BatchStream calibrationStream(
            mParams.calBatchSize, mParams.nbCalBatches, imageDims, listFileName, mParams.dataDirs);
        calibrator.reset(new Int8EntropyCalibrator2<BatchStream>(
            calibrationStream, 0, "UffSSD", mParams.inputTensorNames[0].c_str()));
        config->setFlag(BuilderFlag::kINT8);
        config->setInt8Calibrator(calibrator.get());
    }

    std::cout << "[TensorRT] Build tensorRT engine" << std::endl;
    mEngine = std::shared_ptr<nvinfer1::ICudaEngine>(
                builder->buildEngineWithConfig(*network, *config), samplesCommon::InferDeleter());

    if (!mEngine) {
        std::cout << "[TensorRT] Failed to build tensorRT engine" << std::endl;
        return false;
    }
    else {
        // Create RAII buffer manager object
        buffers = new samplesCommon::BufferManager(mEngine, mParams.batchSize);
        context = SampleUniquePtr<nvinfer1::IExecutionContext>(mEngine->createExecutionContext());
        if (!context) {
            return false;
        }
        std::cout << "[TensorRT] tensorRT engine build complete" << std::endl;
        return true;
    }
}

//!
//! \brief Runs the TensorRT inference engine for this sample
//!
//! \details This function is the main execution function of the sample. It allocates the buffer,
//!          sets inputs, executes the engine and verifies the detection outputs.
//!
bool TensorRT_module::infer(cv::Mat image) {
    // Read the input data into the managed buffers
    assert(mParams.inputTensorNames.size() == 1);
    if (!processInput(*buffers, image)) {
        return false;
    }

    // Memcpy from host input buffers to device input buffers
    buffers->copyInputToDevice();
    const bool status = context->execute(mParams.batchSize, buffers->getDeviceBindings().data());
    if (!status) {
        return false;
    }

    // Memcpy from device output buffers to host output buffers
    buffers->copyOutputToHost();

    // Post-process detections and verify results
    if (!verifyOutput(*buffers)) {
        return false;
    }

    return true;
}

bool TensorRT_module::processInput(const samplesCommon::BufferManager& buffers, cv::Mat image) {
    int batchSize = mParams.batchSize;
    int inputH = mInputDims.d[0];
    int inputW = mInputDims.d[1];
    int inputC = mInputDims.d[2];

    float* hostDataBuffer = static_cast<float*>(buffers.getHostBuffer(mParams.inputTensorNames[0]));
    // Host memory for input buffer
    for (int32_t i = 0; i < batchSize; i++) {               // Batch size (Always set to 1)
        for (int32_t h = 0; h < inputH; h++) {              // Height
            for (int32_t w = 0; w < inputW; w++) {          // Width
                for (int32_t c = 0; c < inputC; c++) {      // Channel
                    int HWC_idx = (inputW * inputC) * h + inputC * w + c;
                    hostDataBuffer[HWC_idx] = image.at<float>(h, w); // Width 방향부터, 그 이후 Height 방향
                }
            }
        }
    }

    return true;
}

//!
//! \brief Filters output detections and verify result
//!
//! \return whether the detection output matches expectations
//!
bool TensorRT_module::verifyOutput(const samplesCommon::BufferManager& buffers) {
    const int32_t inputH = mInputDims.d[0];
    const int32_t inputW = mInputDims.d[1];
    const int32_t batchSize = mParams.batchSize;
    const int32_t keepTopK = mParams.keepTopK;
    const float visualThreshold = mParams.visualThreshold;
    const int32_t outputClsSize = mParams.outputClsSize;

    // Softmax probability
    const float* output = static_cast<const float*>(buffers.getHostBuffer(mParams.outputTensorNames[0]));
    float output_max = 0.0;
    for (int i = 0; i < mParams.outputClsSize; i++) {
        if (output[i] >= output_max) {
            output_max = output[i];
            estimation = i + 1;
        }
    }

    return true;
}

//!
//! \brief Used to clean up any state created in the sample class
//!
bool TensorRT_module::teardown() {
    nvuffparser::shutdownProtobufLibrary();
    return true;
}

//!
//! \brief Initializes members of the params struct
//!        using the command line args
//!
void TensorRT_module::initializeSampleParams() {
    std::cout << "[TensorRT] Initialize tensorRT" << std::endl;

    classes.push_back("Wrist Flexion");     // 0
    classes.push_back("Wrist Extension");   // 1
    classes.push_back("Radial Deviation");  // 2
    classes.push_back("Ulnar Deviation");   // 3
    classes.push_back("Wrist Pronation");   // 4
    classes.push_back("Wrist Supination");  // 5
    classes.push_back("Hand Close");        // 6
    classes.push_back("Hand Open");         // 7

    mParams.dataDirs.push_back("../TRT_demo/tensorRT/"); // release directory
    mParams.uffFileName = "Classifier.uff";
    mParams.labelsFileName = "Classes.txt";
    mParams.inputTensorNames.push_back("inputs"); // Input tensor(layer) name
    mParams.batchSize = 1;
    mParams.outputTensorNames.push_back("Identity"); // First output tensor(layer) name
    mParams.dlaCore = -1;
    mParams.outputClsSize = 8; // 8 Classes
    mParams.visualThreshold = 0.5; // IOU threshold
    mParams.keepTopK = 200; // From python code (tf-ssd-master/models/decoder/SSDDecoder/__init__ default value
    mParams.int8 = false;
    /*FP16 is supported but at a low rate. So performance won't be interesting. The driver version you have should be fine. I would recommend using CUDA 8.0.61 (CUDA 8 GA2) which is what is currently publicly available.
        The only GPUs with full-rate FP16 performance are Tesla P100, Quadro GP100, and Jetson TX1/TX2.
        All GPUs with compute capability 6.1 (e.g. GTX 1050, 1060, 1070, 1080, Pascal Titan X, Titan Xp, Tesla P40, etc.) have low-rate FP16 performance. It?셲 not the fast path on these GPUs. All of these GPUs should support ?쐄ull rate??INT8 performance, however.*/
    mParams.fp16 = true;

    // Calibration parameters (Only valid when int8 == true)
    mParams.calBatchSize = 1;
    mParams.nbCalBatches = 1;
}

void TensorRT_module::initializeVariables() {

}

int TensorRT_module::get_estimation() {
    return estimation;
}
