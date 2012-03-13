/*=============================================================================

 NiftyLink:  A software library to facilitate communication over OpenIGTLink.

             http://cmic.cs.ucl.ac.uk/
             http://www.ucl.ac.uk/

 Copyright (c) UCL : See LICENSE.txt in the top level directory for details.

 Last Changed      : $Date: 2010-05-25 17:02:50 +0100 (Tue, 25 May 2010) $
 Revision          : $Revision: 3300 $
 Last modified by  : $Author: mjc $

 Original author   : m.clarkson@ucl.ac.uk

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 ============================================================================*/

#ifndef OIGTLIMAGEMESSAGE_H
#define OIGTLIMAGEMESSAGE_H

#include <QtGui/QImage>
#include <QtGui/QPainter>

#include "OIGTLMessage.h"
#include "NiftyLinkUtils.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

class NIFTYLINKCOMMON_WINEXPORT OIGTLImageMessage : public OIGTLMessage
{

public:

	typedef OIGTLImageMessage		                      Self;
	typedef QExplicitlySharedDataPointer<Self>        Pointer;
	typedef QExplicitlySharedDataPointer<const Self>  ConstPointer;

	OIGTLImageMessage(void);
	~OIGTLImageMessage(void);
	OIGTLImageMessage(const OIGTLImageMessage &other); 

	static void Create_GET(OIGTLMessage::Pointer &msgToCreate);
	static void Create_STT(OIGTLMessage::Pointer &msgToCreate);
	static void Create_STP(OIGTLMessage::Pointer &msgToCreate);
	static void Create_RTS(OIGTLMessage::Pointer &msgToCreate);

	//    TODO: Add these functions
  /*
    // Image dimensions.
  // SetDimensions() should be called prior to SetSubVolume(), since SetDimensions()
  // sets subvolume parameters automatically assuming that subvolume = entire volume.
  void SetDimensions(int s[3]);
  void SetDimensions(int i, int j, int k);
  void GetDimensions(int s[3]);
  void GetDimensions(int &i, int &j, int &k);

  // Sub-volume dimensions and offset.
  // SetSubVolume() should be called after calling SetDiemensions(), since SetDimensions()
  // reset the subvolume parameters automatically.
  int  SetSubVolume(int dim[3], int off[3]);
  int  SetSubVolume(int dimi, int dimj, int dimk, int offi, int offj, int offk);
  void GetSubVolume(int dim[3], int off[3]);
  void GetSubVolume(int &dimi, int &dimj, int &dimk, int &offi, int &offj, int &offk);

    // Number of components
  void SetNumComponents(int num);
  int  GetNumComponents();
  */
  
  
  void setMatrix(igtl::Matrix4x4 &matrix);
	void getMatrix(igtl::Matrix4x4 &matrix);

	void setNormals(float t[3], float s[3], float n[3]);
	void getNormals(float t[3], float s[3], float n[3]);

  // Spacings
  void setSpacing(float s[3]);
  void setSpacing(float si, float sj, float sk);
  void getSpacing(float s[3]);
  void getSpacing(float &si, float &sj, float &sk);

  // Origin coordinate
  void setOrigin(float p[3]);
  void setOrigin(float px, float py, float pz);
  void getOrigin(float p[3]);
  void getOrigin(float &px, float &py, float &pz);

  void load(QString filename);
  void save(QString filename);

  void setQImage(QImage image);
  QImage getQImage(void);

	virtual void initializeWithTestData(void);
	virtual void initializeWithRandomData(void);

protected:

private:

};

#endif
