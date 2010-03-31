/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/

#include "cppluginconfigreader.h"
#include <qfile.h>
#include <qxmlstreamreader>
#include <cplogger.h>

const QString CHILD_PLUGINS_TAG         = "childplugins";
const QString PLUGIN_TAG                = "plugin";
const QString PLUGIN_ID_ATTR            = "id";
const QString PLUGIN_DLL_ATTR           = "dll";
const QString PLUGIN_DISPALYNAME_ATTR   = "displayname";
const QString DESC_TAG                  = "desc";

CpPluginConfigReader::CpPluginConfigReader(const QString &configPath)
: mConfigPath (configPath)
{
}

CpPluginConfigReader::~CpPluginConfigReader()
{
}

QList<CpPluginConfig> CpPluginConfigReader::readCpPluginConfigs()
{ 
    CpLogger logger;
    
    logger << "reading cpcfg file:" << mConfigPath << "\r\n";
    
    if (mConfigPath.isNull() || mConfigPath.isEmpty()) {
        logger << "CpPluginConfigReader::readCpPluginConfigs() mConfigPath is empty.\r\n";
        return QList<CpPluginConfig> ();
    }

    QFile file(mConfigPath);
    
    if (!file.exists()) {
        logger << mConfigPath << " does not exist.\r\n";
        return QList<CpPluginConfig> ();
    }
    
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        logger << "CpPluginConfigReader::readCpPluginConfigs() open file failed. Error:"
               << static_cast<int>(file.error()) << "\r\n";
        return QList<CpPluginConfig> ();
    }

    QXmlStreamReader reader(&file);

    QList<CpPluginConfig> cpPluginConfigList;

    readCpPluginConfigs(reader, cpPluginConfigList);

    file.close();

    return cpPluginConfigList;
}

void CpPluginConfigReader::readCpPluginConfigs(QXmlStreamReader &xmlReader,QList<CpPluginConfig> &cpPluginConfigList)
{
    xmlReader.readNext();

    while (!xmlReader.atEnd()) {
        
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == CHILD_PLUGINS_TAG) {
                readChildPluginsElement(xmlReader, cpPluginConfigList);
            }
            else {
                xmlReader.raiseError("Not a valid file with the right format.");
            }
        }
        
        else {
            xmlReader.readNext();
        }
    }
}

void CpPluginConfigReader::readChildPluginsElement(QXmlStreamReader &xmlReader,QList<CpPluginConfig> &cpPluginConfigList)
{
    xmlReader.readNext();

    while (!xmlReader.atEnd()) {
        
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == PLUGIN_TAG) {
                readPluginElement(xmlReader, cpPluginConfigList);
            }
            else {
                skipUnknownElement(xmlReader);
            }
        }
        
        else {
            xmlReader.readNext();
        }
    }
}

void CpPluginConfigReader::readPluginElement(QXmlStreamReader &xmlReader,QList<CpPluginConfig> &cpPluginConfigList)
{ 
    CpPluginConfig cpPluginConfig;
  
    QXmlStreamAttributes xmlAttributes = xmlReader.attributes();
  
    if (xmlAttributes.hasAttribute(PLUGIN_ID_ATTR))  {
        cpPluginConfig.mUid 
            = (xmlAttributes.value(PLUGIN_ID_ATTR)).toString().toUInt(0,16);
    }
  
    if (xmlAttributes.hasAttribute(PLUGIN_DLL_ATTR)) {
        cpPluginConfig.mPluginFile 
            = (xmlAttributes.value(PLUGIN_DLL_ATTR)).toString();
    }
  
    if (xmlAttributes.hasAttribute(PLUGIN_DISPALYNAME_ATTR)) {
        cpPluginConfig.mDisplayName 
            = (xmlAttributes.value(PLUGIN_DISPALYNAME_ATTR)).toString();
    }
  
    readDescElement(xmlReader,cpPluginConfig);
  
    cpPluginConfig.dump();
  
    cpPluginConfigList.append(cpPluginConfig);
}

void CpPluginConfigReader::readDescElement(QXmlStreamReader &xmlReader,CpPluginConfig &cpPluginConfig)
{
    xmlReader.readNext();

    while (!xmlReader.atEnd()) {
        
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == DESC_TAG) {
                cpPluginConfig.mDescription = xmlReader.readElementText();
                if (xmlReader.isEndElement()) {
                    xmlReader.readNext();
                }
            }
        }
        
        else {
            xmlReader.readNext();
        }
    }
}

void CpPluginConfigReader::skipUnknownElement(QXmlStreamReader &xmlReader)
{
    xmlReader.readNext();

    while (!xmlReader.atEnd()) {
        
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement()) {
            skipUnknownElement(xmlReader);
        }
        else {
            xmlReader.readNext();
        }
    }
}
