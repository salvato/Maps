/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
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

#include "mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_view(new QWebEngineView(this))
{
    setCentralWidget(m_view);

    QWebEnginePage *page = m_view->page();

    connect(page, SIGNAL(featurePermissionRequested(QUrl, QWebEnginePage::Feature)),
            this, SLOT(onFeaturePermissionRequested(QUrl, QWebEnginePage::Feature)));

//    Display a map (see: https://developers.google.com/maps/documentation/urls/guide)
//
//    The map action returns a map with no markers or directions.
//
//    Forming the map URL
//    https://www.google.com/maps/@?api=1&map_action=map&parameters
//
//    Parameters
//
//        map_action=map (required):
//          Specifies the type of map view to display. Maps and Street View share the same endpoint.
//          To ensure a map is displayed, the map_action must be specified as map.
//
//        center (optional):
//          Defines the center of the map window, and accepts latitude/longitude coordinates
//          as comma-separated values (for example, -33.8569,151.2152).
//
//        zoom (optional):
//          Sets the initial zoom level of the map.
//          Accepted values are whole integers ranging from 0 (the whole world) to 21 (individual buildings).
//          The upper limit can vary depending on the map data available at the selected location. The default is 15.
//
//        basemap (optional):
//          Defines the type of map to display.
//          The value can be either roadmap (default), satellite, or terrain.
//
//        layer (optional):
//          Defines an extra layer to display on the map, if any.
//          The value can be one of the following: none (default), transit, traffic, bicycling or terrain.

    double latitude  = 38.221245;
    double longitude = 15.560085;
    int     zoom     = 21;

#ifdef GOOGLE
    sBaseMap= "satellite";
    sUrl = QString("https://maps.google.com/maps/@?api=1&map_action=map");
    sParameters= QString("&center=%1,%2&zoom=%3&basemap=%4")
                         .arg(latitude,  16)
                         .arg(longitude, 16)
                         .arg(zoom)
                         .arg(sBaseMap);
#else
    sBaseMap= "map";
    sUrl = QString("https://www.openstreetmap.org/search");
    sParameters = QString("?query=#%1=%2/%3/%4/")
                          .arg(sBaseMap)
                          .arg(zoom)
                          .arg(latitude,  16)
                          .arg(longitude, 16);
#endif
    sUrl+= sParameters;
    page->load(sUrl);
}


void
MainWindow::onFeaturePermissionRequested(const QUrl &securityOrigin, QWebEnginePage::Feature feature) {
    if (feature != QWebEnginePage::Geolocation)
        return;
    auto* page = qobject_cast<QWebEnginePage *>(sender());

    QMessageBox msgBox(this);
    msgBox.setText(tr("%1 wants to know your location").arg(securityOrigin.host()));
    msgBox.setInformativeText(tr("Do you want to send your current location to this website?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    if (msgBox.exec() == QMessageBox::Yes) {
        page->setFeaturePermission(
            securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
    } else {
        page->setFeaturePermission(
            securityOrigin, feature, QWebEnginePage::PermissionDeniedByUser);
    }
}
