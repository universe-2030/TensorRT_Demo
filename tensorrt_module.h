#ifndef TENSORRT_MODULE_H
#define TENSORRT_MODULE_H

#include "BatchStream.h"
#include "EntropyCalibrator.h"
#include "argsParser.h"
#include "buffers.h"
#include "common.h"
#include "logger.h"

#include "NvInfer.h"
#include "NvInferPluginUtils.h"
#include "NvUffParser.h"
#include <cuda_runtime_api.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <vector>
#include <cmath>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QTime>

#define TOTAL_FEATURE_MAP 6
#define N_MOTIONS 4

#define IMAGE_H 4
#define IMAGE_W 7
#define IMAGE_C 1

struct SampleUffSSDParams : public samplesCommon::SampleParams {
    std::string uffFileName;    //!< The file name of the UFF model to use
    std::string labelsFileName; //!< The file namefo the class labels
    int32_t outputClsSize;      //!< The number of output classes
    int32_t calBatchSize;       //!< The size of calibration batch
    int32_t nbCalBatches;       //!< The number of batches for calibration
    int32_t keepTopK;           //!< The maximum number of detection post-NMS
    float visualThreshold;      //!< The minimum score threshold to consider a detection
};

class TensorRT_module {
    template <typename T>
    using SampleUniquePtr = std::unique_ptr<T, samplesCommon::InferDeleter>;
public:
    TensorRT_module();
    TensorRT_module(std::string Name_Network);
    ~TensorRT_module();

    //!
    //! \brief Builds the network engine
    //!
    bool build();

    //!
    //! \brief Runs the TensorRT inference engine for this sample
    //!
    bool infer(cv::Mat image);

    //!
    //! \brief Used to clean up any state created in the sample class
    //!
    bool teardown();

    std::vector<std::string> get_class_names();
    int get_estimation();

private:
    SampleUffSSDParams mParams; //!< The parameters for the sample.
    nvinfer1::Dims mInputDims;
    std::vector<samplesCommon::vPPM> mPPMs; //!< PPMs of test images
    std::shared_ptr<nvinfer1::ICudaEngine> mEngine; //!< The TensorRT engine used to run the network
    samplesCommon::BufferManager* buffers;
    SampleUniquePtr<nvinfer1::IExecutionContext> context;
    std::vector<std::string> classes;
    int estimation;

    // Dynamic allocation
    void initializeSampleParams();
    void initializeVariables();
    //!
    //! \brief Parses an UFF model for SSD and creates a TensorRT network
    //!
    bool constructNetwork(SampleUniquePtr<nvinfer1::IBuilder>& builder,
                          SampleUniquePtr<nvinfer1::INetworkDefinition>& network,
                          SampleUniquePtr<nvinfer1::IBuilderConfig>& config,
                          SampleUniquePtr<nvuffparser::IUffParser>& parser);

    //!
    //! \brief Reads the input and mean data, preprocesses, and stores the result in a managed buffer
    //!
    bool processInput(const samplesCommon::BufferManager& buffers, cv::Mat image);

    //!
    //! \brief Filters output detections and verify results
    //!
    bool verifyOutput(const samplesCommon::BufferManager& buffers);
};

#endif // TENSORRT_MODULE_H
