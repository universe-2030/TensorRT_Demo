/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "scenemodifier.h"

#include <QtCore/QDebug>

SceneModifier::SceneModifier(Qt3DCore::QEntity *rootEntity)
    : m_rootEntity(rootEntity) {

    // Vector assignment
    TrunkVec = QVector3D(2.6f, -1.0f, 0.0f);
    UpperArmVec = QVector3D(0.0f, -3.0f, 0.0f);
    LowerArmVec = QVector3D(0.0f, -2.9f, 0.0f);

    ThumbVec_Jnt1 = QVector3D(0.0f, -0.35f, -0.30f);
    ThumbVec_Jnt2 = QVector3D(0.0f, -0.20f, 0.0f);

    IndexVec_Jnt1 = QVector3D(-0.01f, -0.60f, -0.15f);
    IndexVec_Jnt2 = QVector3D(0.0f, -0.243f, 0.0f);
    IndexVec_Jnt3 = QVector3D(0.0f, -0.21f, 0.0f);

    MiddleVec_Jnt1 = QVector3D(-0.03f, -0.63f, 0.02f);
    MiddleVec_Jnt2 = QVector3D(0.0f, -0.27f, 0.0f);
    MiddleVec_Jnt3 = QVector3D(0.0f, -0.22f, 0.0f);

    RingVec_Jnt1 = QVector3D(-0.02f, -0.60f, 0.18f);
    RingVec_Jnt2 = QVector3D(0.0f, -0.243f, 0.0f);
    RingVec_Jnt3 = QVector3D(0.0f, -0.21f, 0.0f);

    LittleVec_Jnt1 = QVector3D(-0.02f, -0.52f, 0.35f);
    LittleVec_Jnt2 = QVector3D(0.0f, -0.17f, 0.0f);
    LittleVec_Jnt3 = QVector3D(0.0f, -0.157f, 0.0f);

    ////////////////////// 1. Trunk //////////////////////
    Qt3DRender::QMesh *mesh_trunk = new Qt3DRender::QMesh();
    mesh_trunk->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Trunk.obj"));

    Qt3DExtras::QPhongMaterial *TrunkMaterial = new Qt3DExtras::QPhongMaterial();
    TrunkMaterial->setDiffuse(QColor(QRgb(0xbe8e77)));

    TrunkTransform[0] = new Qt3DCore::QTransform();
    TrunkTransform[0]->setScale(1.0f);
    TrunkTransform[0]->setRotationX(180);

    m_TrunkEntity[0] = new Qt3DCore::QEntity(m_rootEntity);
    m_TrunkEntity[0]->addComponent(TrunkTransform[0]);

    TrunkTransform[1] = new Qt3DCore::QTransform();
    TrunkTransform[1]->setRotationY(0);

    m_TrunkEntity[1] = new Qt3DCore::QEntity(m_TrunkEntity[0]);
    m_TrunkEntity[1]->addComponent(TrunkTransform[1]);

    TrunkTransform[2] = new Qt3DCore::QTransform();
    TrunkTransform[2]->setRotationZ(0);

    m_TrunkEntity[2] = new Qt3DCore::QEntity(m_TrunkEntity[1]);
    m_TrunkEntity[2]->addComponent(mesh_trunk);
    m_TrunkEntity[2]->addComponent(TrunkMaterial);
    m_TrunkEntity[2]->addComponent(TrunkTransform[2]);

    ////////////////////// 2. Upper arm //////////////////////
    Qt3DRender::QMesh *mesh_upper_arm = new Qt3DRender::QMesh();
    mesh_upper_arm->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Upper_arm.obj"));

    Qt3DExtras::QPhongMaterial *UpperArmMaterial = new Qt3DExtras::QPhongMaterial();
    UpperArmMaterial->setDiffuse(QColor(QRgb(0xbe8e77)));

    UpperArmTransform[0] = new Qt3DCore::QTransform();
    UpperArmTransform[0]->setScale(1.0f);
    UpperArmTransform[0]->setTranslation(TrunkVec);

    m_UpperArmEntity[0] = new Qt3DCore::QEntity(m_TrunkEntity[2]);
    m_UpperArmEntity[0]->addComponent(UpperArmTransform[0]);

    UpperArmTransform[1] = new Qt3DCore::QTransform();
    UpperArmTransform[1]->setRotationX(180);

    m_UpperArmEntity[1] = new Qt3DCore::QEntity(m_UpperArmEntity[0]);
    m_UpperArmEntity[1]->addComponent(UpperArmTransform[1]);

    UpperArmTransform[2] = new Qt3DCore::QTransform();
    UpperArmTransform[2]->setRotationX(0);

    m_UpperArmEntity[2] = new Qt3DCore::QEntity(m_UpperArmEntity[1]);
    m_UpperArmEntity[2]->addComponent(UpperArmTransform[2]);

    UpperArmTransform[3] = new Qt3DCore::QTransform();
    ax_X = 0.0;
    ax_Y = -cos(SHD_X_ROM_DYNAMIC * PI / 180.0);
    ax_Z = sin(SHD_X_ROM_DYNAMIC * PI / 180.0);
    QQuaternion temp = UpperArmTransform[3]->fromAxisAndAngle(ax_X, ax_Y, ax_Z, 0.0);
    UpperArmTransform[3]->setRotation(temp);

    m_UpperArmEntity[3] = new Qt3DCore::QEntity(m_UpperArmEntity[2]);
    m_UpperArmEntity[3]->addComponent(UpperArmTransform[3]);

    UpperArmTransform[4] = new Qt3DCore::QTransform();
    UpperArmTransform[4]->setRotationY(0);

    m_UpperArmEntity[4] = new Qt3DCore::QEntity(m_UpperArmEntity[3]);
    m_UpperArmEntity[4]->addComponent(UpperArmTransform[4]);

    UpperArmTransform[5] = new Qt3DCore::QTransform();
    UpperArmTransform[5]->setRotationZ(0);

    m_UpperArmEntity[5] = new Qt3DCore::QEntity(m_UpperArmEntity[4]);
    m_UpperArmEntity[5]->addComponent(mesh_upper_arm);
    m_UpperArmEntity[5]->addComponent(UpperArmMaterial);
    m_UpperArmEntity[5]->addComponent(UpperArmTransform[5]);

    ////////////////////// 3. Lower arm //////////////////////
    Qt3DRender::QMesh *mesh_lower_arm = new Qt3DRender::QMesh();
    mesh_lower_arm->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Lower_arm.obj"));

    Qt3DExtras::QPhongMaterial *LowerArmMaterial = new Qt3DExtras::QPhongMaterial();
    LowerArmMaterial->setDiffuse(QColor(QRgb(0xbe8e77)));

    LowerArmTransform[0] = new Qt3DCore::QTransform();
    LowerArmTransform[0]->setScale(1.0f);
    LowerArmTransform[0]->setTranslation(UpperArmVec);

    m_LowerArmEntity[0] = new Qt3DCore::QEntity(m_UpperArmEntity[5]);
    m_LowerArmEntity[0]->addComponent(LowerArmTransform[0]);

    LowerArmTransform[1] = new Qt3DCore::QTransform();
    LowerArmTransform[1]->setRotationX(0); // Elbow sagittal

    m_LowerArmEntity[1] = new Qt3DCore::QEntity(m_LowerArmEntity[0]);
    m_LowerArmEntity[1]->addComponent(LowerArmTransform[1]);

    LowerArmTransform[2] = new Qt3DCore::QTransform();
    LowerArmTransform[2]->setRotationY(0); // Elbow internal rotation

    m_LowerArmEntity[2] = new Qt3DCore::QEntity(m_LowerArmEntity[1]);
    m_LowerArmEntity[2]->addComponent(LowerArmTransform[2]);

    LowerArmTransform[3] = new Qt3DCore::QTransform();
    LowerArmTransform[3]->setRotationZ(0); // Elbow frontal

    m_LowerArmEntity[3] = new Qt3DCore::QEntity(m_LowerArmEntity[2]);
    m_LowerArmEntity[3]->addComponent(mesh_lower_arm);
    m_LowerArmEntity[3]->addComponent(LowerArmMaterial);
    m_LowerArmEntity[3]->addComponent(LowerArmTransform[3]);

    //////////////////////// 4. Palm ////////////////////////
    Qt3DRender::QMesh *mesh_palm = new Qt3DRender::QMesh();
    mesh_palm->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/hand.obj"));

    Qt3DExtras::QPhongMaterial *PalmMaterial = new Qt3DExtras::QPhongMaterial();
    PalmMaterial->setDiffuse(QColor(QRgb(0xbe8e77)));

    PalmTransform[0] = new Qt3DCore::QTransform();
    PalmTransform[0]->setScale(2.0f);
    PalmTransform[0]->setTranslation(LowerArmVec);

    m_PalmEntity[0] = new Qt3DCore::QEntity(m_LowerArmEntity[3]);
    m_PalmEntity[0]->addComponent(PalmTransform[0]);

    PalmTransform[1] = new Qt3DCore::QTransform();
    PalmTransform[1]->setRotationX(10);

    m_PalmEntity[1] = new Qt3DCore::QEntity(m_PalmEntity[0]);
    m_PalmEntity[1]->addComponent(PalmTransform[1]);

    PalmTransform[2] = new Qt3DCore::QTransform();
    PalmTransform[2]->setRotationY(0);

    m_PalmEntity[2] = new Qt3DCore::QEntity(m_PalmEntity[1]);
    m_PalmEntity[2]->addComponent(PalmTransform[2]);

    PalmTransform[3] = new Qt3DCore::QTransform();
    PalmTransform[3]->setRotationZ(15);

    m_PalmEntity[3] = new Qt3DCore::QEntity(m_PalmEntity[2]);
    m_PalmEntity[3]->addComponent(mesh_palm);
    m_PalmEntity[3]->addComponent(PalmMaterial);
    m_PalmEntity[3]->addComponent(PalmTransform[3]);

    // 5-1. Thumb
    Qt3DExtras::QSphereMesh *mesh_thumb_jnt1 = new Qt3DExtras::QSphereMesh();
    mesh_thumb_jnt1->setRadius(0.05);
    mesh_thumb_jnt1->setRings(20);
    mesh_thumb_jnt1->setSlices(20);

    Qt3DExtras::QSphereMesh *mesh_thumb_jnt2 = new Qt3DExtras::QSphereMesh();
    mesh_thumb_jnt2->setRadius(0.05);
    mesh_thumb_jnt2->setRings(20);
    mesh_thumb_jnt2->setSlices(20);

    Qt3DRender::QMesh *mesh_thumb_phl1 = new Qt3DRender::QMesh();
    mesh_thumb_phl1->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_thumb_1.obj"));

    Qt3DRender::QMesh *mesh_thumb_phl2 = new Qt3DRender::QMesh();
    mesh_thumb_phl2->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_thumb_2.obj"));

    Qt3DExtras::QPhongMaterial *ThumbMaterial = new Qt3DExtras::QPhongMaterial();
    ThumbMaterial->setDiffuse(QColor(QRgb(0xbe8e77)));

    ThumbTransform[0] = new Qt3DCore::QTransform();
    ThumbTransform[0]->setTranslation(ThumbVec_Jnt1);

    ThumbTransform[1] = new Qt3DCore::QTransform();
    ThumbTransform[1]->setRotationX(40);

    ThumbTransform[2] = new Qt3DCore::QTransform();
    ThumbTransform[2]->setRotationZ(-10);

    ThumbTransform[3] = new Qt3DCore::QTransform();
    ThumbTransform[3]->setTranslation(ThumbVec_Jnt2);

    ThumbTransform[4] = new Qt3DCore::QTransform();
    ThumbTransform[4]->setRotationZ(-10);

    m_ThumbEntity[0] = new Qt3DCore::QEntity(m_PalmEntity[3]);
    m_ThumbEntity[0]->addComponent(mesh_thumb_jnt1);
    m_ThumbEntity[0]->addComponent(ThumbMaterial);
    m_ThumbEntity[0]->addComponent(ThumbTransform[0]);

    m_ThumbEntity[1] = new Qt3DCore::QEntity(m_ThumbEntity[0]);
    m_ThumbEntity[1]->addComponent(ThumbTransform[1]);

    m_ThumbEntity[2] = new Qt3DCore::QEntity(m_ThumbEntity[1]);
    m_ThumbEntity[2]->addComponent(mesh_thumb_phl1);
    m_ThumbEntity[2]->addComponent(ThumbMaterial);
    m_ThumbEntity[2]->addComponent(ThumbTransform[2]);

    m_ThumbEntity[3] = new Qt3DCore::QEntity(m_ThumbEntity[2]);
    m_ThumbEntity[3]->addComponent(mesh_thumb_jnt2);
    m_ThumbEntity[3]->addComponent(ThumbMaterial);
    m_ThumbEntity[3]->addComponent(ThumbTransform[3]);

    m_ThumbEntity[4] = new Qt3DCore::QEntity(m_ThumbEntity[3]);
    m_ThumbEntity[4]->addComponent(mesh_thumb_phl2);
    m_ThumbEntity[4]->addComponent(ThumbMaterial);
    m_ThumbEntity[4]->addComponent(ThumbTransform[4]);

    // 5-2. Index
    Qt3DExtras::QSphereMesh *mesh_index_jnt1 = new Qt3DExtras::QSphereMesh();
    mesh_index_jnt1->setRadius(0.05);
    mesh_index_jnt1->setRings(20);
    mesh_index_jnt1->setSlices(20);

    Qt3DExtras::QSphereMesh *mesh_index_jnt2 = new Qt3DExtras::QSphereMesh();
    mesh_index_jnt2->setRadius(0.05);
    mesh_index_jnt2->setRings(20);
    mesh_index_jnt2->setSlices(20);

    Qt3DExtras::QSphereMesh *mesh_index_jnt3 = new Qt3DExtras::QSphereMesh();
    mesh_index_jnt3->setRadius(0.05);
    mesh_index_jnt3->setRings(20);
    mesh_index_jnt3->setSlices(20);

    Qt3DRender::QMesh *mesh_index_phl1 = new Qt3DRender::QMesh();
    mesh_index_phl1->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_index_1.obj"));

    Qt3DRender::QMesh *mesh_index_phl2 = new Qt3DRender::QMesh();
    mesh_index_phl2->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_index_2.obj"));

    Qt3DRender::QMesh *mesh_index_phl3 = new Qt3DRender::QMesh();
    mesh_index_phl3->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_index_3.obj"));

    Qt3DExtras::QPhongMaterial *IndexMaterial = new Qt3DExtras::QPhongMaterial();
    IndexMaterial->setDiffuse(QColor(QRgb(0xbe8e77)));

    IndexTransform[0] = new Qt3DCore::QTransform();
    IndexTransform[0]->setTranslation(IndexVec_Jnt1);

    IndexTransform[1] = new Qt3DCore::QTransform();
    IndexTransform[1]->setRotationX(10);

    IndexTransform[2] = new Qt3DCore::QTransform();
    IndexTransform[2]->setRotationZ(-10);

    IndexTransform[3] = new Qt3DCore::QTransform();
    IndexTransform[3]->setTranslation(IndexVec_Jnt2);

    IndexTransform[4] = new Qt3DCore::QTransform();
    IndexTransform[4]->setRotationZ(-10);

    IndexTransform[5] = new Qt3DCore::QTransform();
    IndexTransform[5]->setTranslation(IndexVec_Jnt3);

    IndexTransform[6] = new Qt3DCore::QTransform();
    IndexTransform[6]->setRotationZ(-10);

    m_IndexEntity[0] = new Qt3DCore::QEntity(m_PalmEntity[3]);
    m_IndexEntity[0]->addComponent(mesh_index_jnt1);
    m_IndexEntity[0]->addComponent(IndexMaterial);
    m_IndexEntity[0]->addComponent(IndexTransform[0]);

    m_IndexEntity[1] = new Qt3DCore::QEntity(m_IndexEntity[0]);
    m_IndexEntity[1]->addComponent(IndexTransform[1]);

    m_IndexEntity[2] = new Qt3DCore::QEntity(m_IndexEntity[1]);
    m_IndexEntity[2]->addComponent(mesh_index_phl1);
    m_IndexEntity[2]->addComponent(IndexMaterial);
    m_IndexEntity[2]->addComponent(IndexTransform[2]);

    m_IndexEntity[3] = new Qt3DCore::QEntity(m_IndexEntity[2]);
    m_IndexEntity[3]->addComponent(mesh_index_jnt2);
    m_IndexEntity[3]->addComponent(IndexMaterial);
    m_IndexEntity[3]->addComponent(IndexTransform[3]);

    m_IndexEntity[4] = new Qt3DCore::QEntity(m_IndexEntity[3]);
    m_IndexEntity[4]->addComponent(mesh_index_phl2);
    m_IndexEntity[4]->addComponent(IndexMaterial);
    m_IndexEntity[4]->addComponent(IndexTransform[4]);

    m_IndexEntity[5] = new Qt3DCore::QEntity(m_IndexEntity[4]);
    m_IndexEntity[5]->addComponent(mesh_index_jnt3);
    m_IndexEntity[5]->addComponent(IndexMaterial);
    m_IndexEntity[5]->addComponent(IndexTransform[5]);

    m_IndexEntity[6] = new Qt3DCore::QEntity(m_IndexEntity[5]);
    m_IndexEntity[6]->addComponent(mesh_index_phl3);
    m_IndexEntity[6]->addComponent(IndexMaterial);
    m_IndexEntity[6]->addComponent(IndexTransform[6]);

    // 5-3. Middle
    Qt3DExtras::QSphereMesh *mesh_middle_jnt1 = new Qt3DExtras::QSphereMesh();
    mesh_middle_jnt1->setRadius(0.05);
    mesh_middle_jnt1->setRings(20);
    mesh_middle_jnt1->setSlices(20);

    Qt3DExtras::QSphereMesh *mesh_middle_jnt2 = new Qt3DExtras::QSphereMesh();
    mesh_middle_jnt2->setRadius(0.05);
    mesh_middle_jnt2->setRings(20);
    mesh_middle_jnt2->setSlices(20);

    Qt3DExtras::QSphereMesh *mesh_middle_jnt3 = new Qt3DExtras::QSphereMesh();
    mesh_middle_jnt3->setRadius(0.05);
    mesh_middle_jnt3->setRings(20);
    mesh_middle_jnt3->setSlices(20);

    Qt3DRender::QMesh *mesh_middle_phl1 = new Qt3DRender::QMesh();
    mesh_middle_phl1->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_middle_1.obj"));

    Qt3DRender::QMesh *mesh_middle_phl2 = new Qt3DRender::QMesh();
    mesh_middle_phl2->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_middle_2.obj"));

    Qt3DRender::QMesh *mesh_middle_phl3 = new Qt3DRender::QMesh();
    mesh_middle_phl3->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_middle_3.obj"));

    Qt3DExtras::QPhongMaterial *MiddleMaterial = new Qt3DExtras::QPhongMaterial();
    MiddleMaterial->setDiffuse(QColor(QRgb(0xbe8e77)));

    MiddleTransform[0] = new Qt3DCore::QTransform();
    MiddleTransform[0]->setTranslation(MiddleVec_Jnt1);

    MiddleTransform[1] = new Qt3DCore::QTransform();
    MiddleTransform[1]->setRotationX(0);

    MiddleTransform[2] = new Qt3DCore::QTransform();
    MiddleTransform[2]->setRotationZ(-10);

    MiddleTransform[3] = new Qt3DCore::QTransform();
    MiddleTransform[3]->setTranslation(MiddleVec_Jnt2);

    MiddleTransform[4] = new Qt3DCore::QTransform();
    MiddleTransform[4]->setRotationZ(-10);

    MiddleTransform[5] = new Qt3DCore::QTransform();
    MiddleTransform[5]->setTranslation(MiddleVec_Jnt3);

    MiddleTransform[6] = new Qt3DCore::QTransform();
    MiddleTransform[6]->setRotationZ(-10);

    m_MiddleEntity[0] = new Qt3DCore::QEntity(m_PalmEntity[3]);
    m_MiddleEntity[0]->addComponent(mesh_middle_jnt1);
    m_MiddleEntity[0]->addComponent(MiddleMaterial);
    m_MiddleEntity[0]->addComponent(MiddleTransform[0]);

    m_MiddleEntity[1] = new Qt3DCore::QEntity(m_MiddleEntity[0]);
    m_MiddleEntity[1]->addComponent(MiddleTransform[1]);

    m_MiddleEntity[2] = new Qt3DCore::QEntity(m_MiddleEntity[1]);
    m_MiddleEntity[2]->addComponent(mesh_middle_phl1);
    m_MiddleEntity[2]->addComponent(MiddleMaterial);
    m_MiddleEntity[2]->addComponent(MiddleTransform[2]);

    m_MiddleEntity[3] = new Qt3DCore::QEntity(m_MiddleEntity[2]);
    m_MiddleEntity[3]->addComponent(mesh_middle_jnt2);
    m_MiddleEntity[3]->addComponent(MiddleMaterial);
    m_MiddleEntity[3]->addComponent(MiddleTransform[3]);

    m_MiddleEntity[4] = new Qt3DCore::QEntity(m_MiddleEntity[3]);
    m_MiddleEntity[4]->addComponent(mesh_middle_phl2);
    m_MiddleEntity[4]->addComponent(MiddleMaterial);
    m_MiddleEntity[4]->addComponent(MiddleTransform[4]);

    m_MiddleEntity[5] = new Qt3DCore::QEntity(m_MiddleEntity[4]);
    m_MiddleEntity[5]->addComponent(mesh_middle_jnt3);
    m_MiddleEntity[5]->addComponent(MiddleMaterial);
    m_MiddleEntity[5]->addComponent(MiddleTransform[5]);

    m_MiddleEntity[6] = new Qt3DCore::QEntity(m_MiddleEntity[5]);
    m_MiddleEntity[6]->addComponent(mesh_middle_phl3);
    m_MiddleEntity[6]->addComponent(MiddleMaterial);
    m_MiddleEntity[6]->addComponent(MiddleTransform[6]);

    // 5-4. Ring
    Qt3DExtras::QSphereMesh *mesh_ring_jnt1 = new Qt3DExtras::QSphereMesh();
    mesh_ring_jnt1->setRadius(0.05);
    mesh_ring_jnt1->setRings(20);
    mesh_ring_jnt1->setSlices(20);

    Qt3DExtras::QSphereMesh *mesh_ring_jnt2 = new Qt3DExtras::QSphereMesh();
    mesh_ring_jnt2->setRadius(0.05);
    mesh_ring_jnt2->setRings(20);
    mesh_ring_jnt2->setSlices(20);

    Qt3DExtras::QSphereMesh *mesh_ring_jnt3 = new Qt3DExtras::QSphereMesh();
    mesh_ring_jnt3->setRadius(0.05);
    mesh_ring_jnt3->setRings(20);
    mesh_ring_jnt3->setSlices(20);

    Qt3DRender::QMesh *mesh_ring_phl1 = new Qt3DRender::QMesh();
    mesh_ring_phl1->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_ring_1.obj"));

    Qt3DRender::QMesh *mesh_ring_phl2 = new Qt3DRender::QMesh();
    mesh_ring_phl2->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_ring_2.obj"));

    Qt3DRender::QMesh *mesh_ring_phl3 = new Qt3DRender::QMesh();
    mesh_ring_phl3->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_ring_3.obj"));

    Qt3DExtras::QPhongMaterial *RingMaterial = new Qt3DExtras::QPhongMaterial();
    RingMaterial->setDiffuse(QColor(QRgb(0xbe8e77)));

    RingTransform[0] = new Qt3DCore::QTransform();
    RingTransform[0]->setTranslation(RingVec_Jnt1);

    RingTransform[1] = new Qt3DCore::QTransform();
    RingTransform[1]->setRotationX(-7);

    RingTransform[2] = new Qt3DCore::QTransform();
    RingTransform[2]->setRotationZ(-10);

    RingTransform[3] = new Qt3DCore::QTransform();
    RingTransform[3]->setTranslation(RingVec_Jnt2);

    RingTransform[4] = new Qt3DCore::QTransform();
    RingTransform[4]->setRotationZ(-10);

    RingTransform[5] = new Qt3DCore::QTransform();
    RingTransform[5]->setTranslation(RingVec_Jnt3);

    RingTransform[6] = new Qt3DCore::QTransform();
    RingTransform[6]->setRotationZ(-10);

    m_RingEntity[0] = new Qt3DCore::QEntity(m_PalmEntity[3]);
    m_RingEntity[0]->addComponent(mesh_ring_jnt1);
    m_RingEntity[0]->addComponent(RingMaterial);
    m_RingEntity[0]->addComponent(RingTransform[0]);

    m_RingEntity[1] = new Qt3DCore::QEntity(m_RingEntity[0]);
    m_RingEntity[1]->addComponent(RingTransform[1]);

    m_RingEntity[2] = new Qt3DCore::QEntity(m_RingEntity[1]);
    m_RingEntity[2]->addComponent(mesh_ring_phl1);
    m_RingEntity[2]->addComponent(RingMaterial);
    m_RingEntity[2]->addComponent(RingTransform[2]);

    m_RingEntity[3] = new Qt3DCore::QEntity(m_RingEntity[2]);
    m_RingEntity[3]->addComponent(mesh_ring_jnt2);
    m_RingEntity[3]->addComponent(RingMaterial);
    m_RingEntity[3]->addComponent(RingTransform[3]);

    m_RingEntity[4] = new Qt3DCore::QEntity(m_RingEntity[3]);
    m_RingEntity[4]->addComponent(mesh_ring_phl2);
    m_RingEntity[4]->addComponent(RingMaterial);
    m_RingEntity[4]->addComponent(RingTransform[4]);

    m_RingEntity[5] = new Qt3DCore::QEntity(m_RingEntity[4]);
    m_RingEntity[5]->addComponent(mesh_ring_jnt3);
    m_RingEntity[5]->addComponent(RingMaterial);
    m_RingEntity[5]->addComponent(RingTransform[5]);

    m_RingEntity[6] = new Qt3DCore::QEntity(m_RingEntity[5]);
    m_RingEntity[6]->addComponent(mesh_ring_phl3);
    m_RingEntity[6]->addComponent(RingMaterial);
    m_RingEntity[6]->addComponent(RingTransform[6]);

    // 5-5. Little
    Qt3DExtras::QSphereMesh *mesh_little_jnt1 = new Qt3DExtras::QSphereMesh();
    mesh_little_jnt1->setRadius(0.05);
    mesh_little_jnt1->setRings(20);
    mesh_little_jnt1->setSlices(20);

    Qt3DExtras::QSphereMesh *mesh_little_jnt2 = new Qt3DExtras::QSphereMesh();
    mesh_little_jnt2->setRadius(0.05);
    mesh_little_jnt2->setRings(20);
    mesh_little_jnt2->setSlices(20);

    Qt3DExtras::QSphereMesh *mesh_little_jnt3 = new Qt3DExtras::QSphereMesh();
    mesh_little_jnt3->setRadius(0.05);
    mesh_little_jnt3->setRings(20);
    mesh_little_jnt3->setSlices(20);

    Qt3DRender::QMesh *mesh_little_phl1 = new Qt3DRender::QMesh();
    mesh_little_phl1->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_little_1.obj"));

    Qt3DRender::QMesh *mesh_little_phl2 = new Qt3DRender::QMesh();
    mesh_little_phl2->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_little_2.obj"));

    Qt3DRender::QMesh *mesh_little_phl3 = new Qt3DRender::QMesh();
    mesh_little_phl3->setSource(QUrl::fromLocalFile("../TRT_demo/3D model/Phalange_little_3.obj"));

    Qt3DExtras::QPhongMaterial *LittleMaterial = new Qt3DExtras::QPhongMaterial();
    LittleMaterial->setDiffuse(QColor(QRgb(0xbe8e77)));

    LittleTransform[0] = new Qt3DCore::QTransform();
    LittleTransform[0]->setTranslation(LittleVec_Jnt1);

    LittleTransform[1] = new Qt3DCore::QTransform();
    LittleTransform[1]->setRotationX(-12);

    LittleTransform[2] = new Qt3DCore::QTransform();
    LittleTransform[2]->setRotationZ(-10);

    LittleTransform[3] = new Qt3DCore::QTransform();
    LittleTransform[3]->setTranslation(LittleVec_Jnt2);

    LittleTransform[4] = new Qt3DCore::QTransform();
    LittleTransform[4]->setRotationZ(-10);

    LittleTransform[5] = new Qt3DCore::QTransform();
    LittleTransform[5]->setTranslation(LittleVec_Jnt3);

    LittleTransform[6] = new Qt3DCore::QTransform();
    LittleTransform[6]->setRotationZ(-10);

    m_LittleEntity[0] = new Qt3DCore::QEntity(m_PalmEntity[3]);
    m_LittleEntity[0]->addComponent(mesh_little_jnt1);
    m_LittleEntity[0]->addComponent(LittleMaterial);
    m_LittleEntity[0]->addComponent(LittleTransform[0]);

    m_LittleEntity[1] = new Qt3DCore::QEntity(m_LittleEntity[0]);
    m_LittleEntity[1]->addComponent(LittleTransform[1]);

    m_LittleEntity[2] = new Qt3DCore::QEntity(m_LittleEntity[1]);
    m_LittleEntity[2]->addComponent(mesh_little_phl1);
    m_LittleEntity[2]->addComponent(LittleMaterial);
    m_LittleEntity[2]->addComponent(LittleTransform[2]);

    m_LittleEntity[3] = new Qt3DCore::QEntity(m_LittleEntity[2]);
    m_LittleEntity[3]->addComponent(mesh_little_jnt2);
    m_LittleEntity[3]->addComponent(LittleMaterial);
    m_LittleEntity[3]->addComponent(LittleTransform[3]);

    m_LittleEntity[4] = new Qt3DCore::QEntity(m_LittleEntity[3]);
    m_LittleEntity[4]->addComponent(mesh_little_phl2);
    m_LittleEntity[4]->addComponent(LittleMaterial);
    m_LittleEntity[4]->addComponent(LittleTransform[4]);

    m_LittleEntity[5] = new Qt3DCore::QEntity(m_LittleEntity[4]);
    m_LittleEntity[5]->addComponent(mesh_little_jnt3);
    m_LittleEntity[5]->addComponent(LittleMaterial);
    m_LittleEntity[5]->addComponent(LittleTransform[5]);

    m_LittleEntity[6] = new Qt3DCore::QEntity(m_LittleEntity[5]);
    m_LittleEntity[6]->addComponent(mesh_little_phl3);
    m_LittleEntity[6]->addComponent(LittleMaterial);
    m_LittleEntity[6]->addComponent(LittleTransform[6]);
}

SceneModifier::~SceneModifier() {

}

// Upper Arm
void SceneModifier::set_Upper_arm_RotX(double angle) {
    UpperArmTransform[2]->setRotationX(angle);
}

void SceneModifier::set_Upper_arm_RotY(double angle) {
    UpperArmTransform[4]->setRotationY(angle);
}

void SceneModifier::set_Upper_arm_RotZ(double angle) {
    UpperArmTransform[5]->setRotationZ(angle);
}

void SceneModifier::set_Upper_arm_Axis(double angle) {
    QQuaternion temp = UpperArmTransform[3]->fromAxisAndAngle(ax_X, ax_Y, ax_Z, angle);
    UpperArmTransform[3]->setRotation(temp);
}

// Lower Arm
void SceneModifier::set_Lower_arm_RotX(double angle) {
    LowerArmTransform[1]->setRotationX(angle);
}

// Wrist movement
void SceneModifier::set_Lower_arm_RotY(double angle) {
    LowerArmTransform[2]->setRotationY(angle);
}

void SceneModifier::set_Palm_RotX(double angle) {
    PalmTransform[1]->setRotationX(angle);
}

void SceneModifier::set_Palm_RotY(double angle) {
    PalmTransform[2]->setRotationY(angle);
}

void SceneModifier::set_Palm_RotZ(double angle) {
    PalmTransform[3]->setRotationZ(angle);
}

// Finger movement
void SceneModifier::set_Thumb_RotZ(double angle) {
    ThumbTransform[2]->setRotationZ(angle);
    ThumbTransform[4]->setRotationZ(angle);
}

void SceneModifier::set_Index_RotZ(double angle) {
    IndexTransform[2]->setRotationZ(angle);
    IndexTransform[4]->setRotationZ(angle);
    IndexTransform[6]->setRotationZ(angle);
}

void SceneModifier::set_Middle_RotZ(double angle) {
    MiddleTransform[2]->setRotationZ(angle);
    MiddleTransform[4]->setRotationZ(angle);
    MiddleTransform[6]->setRotationZ(angle);
}

void SceneModifier::set_Ring_RotZ(double angle) {
    RingTransform[2]->setRotationZ(angle);
    RingTransform[4]->setRotationZ(angle);
    RingTransform[6]->setRotationZ(angle);
}

void SceneModifier::set_Little_RotZ(double angle) {
    LittleTransform[2]->setRotationZ(angle);
    LittleTransform[4]->setRotationZ(angle);
    LittleTransform[6]->setRotationZ(angle);
}
