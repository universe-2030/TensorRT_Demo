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

#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H

#include <QtCore/QObject>
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

#include <Qt3dRender>

#define PI 3.14159265358

// Motion trajectory variables
#define SHD_X_ROM_STATIC 90.0
#define SHD_Z_ROM_STATIC 90.0
#define ELB_X_ROM_STATIC 90.0

#define SHD_X_ROM_DYNAMIC 120.0
#define SHD_Z_ROM_DYNAMIC 120.0
#define ELB_X_ROM_DYNAMIC 120.0

class SceneModifier : public QObject {
    Q_OBJECT

public:
    explicit SceneModifier(Qt3DCore::QEntity *rootEntity);
    ~SceneModifier();

    void set_Upper_arm_RotX(double angle);
    void set_Upper_arm_RotY(double angle);
    void set_Upper_arm_RotZ(double angle);
    void set_Upper_arm_Axis(double angle);

    void set_Lower_arm_RotX(double angle);
    void set_Lower_arm_RotY(double angle); // WP & WS

    void set_Palm_RotX(double angle);      // RD & UD
    void set_Palm_RotY(double angle);      // WP & WS (Reserved)
    void set_Palm_RotZ(double angle);      // WF & WE

    void set_Thumb_RotZ(double angle);     // Thumb Flexion & Extension
    void set_Index_RotZ(double angle);     // Index Flexion & Extension
    void set_Middle_RotZ(double angle);    // Middle Flexion & Extension
    void set_Ring_RotZ(double angle);      // Ring Flexion & Extension
    void set_Little_RotZ(double angle);    // Little Flexion & Extension

private:
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DCore::QEntity *m_TrunkEntity[3];
    Qt3DCore::QEntity *m_UpperArmEntity[6];
    Qt3DCore::QEntity *m_LowerArmEntity[4];
    Qt3DCore::QEntity *m_PalmEntity[4];
    Qt3DCore::QEntity *m_ThumbEntity[5];
    Qt3DCore::QEntity *m_IndexEntity[7];
    Qt3DCore::QEntity *m_MiddleEntity[7];
    Qt3DCore::QEntity *m_RingEntity[7];
    Qt3DCore::QEntity *m_LittleEntity[7];

    Qt3DCore::QTransform *TrunkTransform[3];
    Qt3DCore::QTransform *UpperArmTransform[6];
    Qt3DCore::QTransform *LowerArmTransform[4];
    Qt3DCore::QTransform *PalmTransform[4];
    Qt3DCore::QTransform *ThumbTransform[5];
    Qt3DCore::QTransform *IndexTransform[7];
    Qt3DCore::QTransform *MiddleTransform[7];
    Qt3DCore::QTransform *RingTransform[7];
    Qt3DCore::QTransform *LittleTransform[7];

    QVector3D TrunkVec;
    QVector3D UpperArmVec;
    QVector3D LowerArmVec;

    QVector3D ThumbVec_Jnt1;
    QVector3D ThumbVec_Jnt2;

    QVector3D IndexVec_Jnt1;
    QVector3D IndexVec_Jnt2;
    QVector3D IndexVec_Jnt3;

    QVector3D MiddleVec_Jnt1;
    QVector3D MiddleVec_Jnt2;
    QVector3D MiddleVec_Jnt3;

    QVector3D RingVec_Jnt1;
    QVector3D RingVec_Jnt2;
    QVector3D RingVec_Jnt3;

    QVector3D LittleVec_Jnt1;
    QVector3D LittleVec_Jnt2;
    QVector3D LittleVec_Jnt3;

    double ax_X = 0.0;
    double ax_Y = -1.0;
    double ax_Z = 0.0;
};

#endif // SCENEMODIFIER_H

