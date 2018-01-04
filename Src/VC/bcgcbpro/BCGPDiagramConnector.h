//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2012 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPDiagramConnector.h: interface for the CBCGPDiagramConnector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPDIAGRAMCONNECTOR_H__D398297F_D298_496B_B7CF_25A1BE568972__INCLUDED_)
#define AFX_BCGPDIAGRAMCONNECTOR_H__D398297F_D298_496B_B7CF_25A1BE568972__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGPDiagramVisualObject.h"

//////////////////////////////////////////////////////////////////////
// CBCGPDiagramShelfConnector

class BCGCBPRODLLEXPORT CBCGPDiagramShelfConnector : public CBCGPDiagramConnector
{
	DECLARE_DYNAMIC(CBCGPDiagramShelfConnector)

	friend class CBCGPVisualConstructor;

public:
	CBCGPDiagramShelfConnector(double dShelfOffset = 10.0);
	CBCGPDiagramShelfConnector(const CBCGPPoint& pt1, const CBCGPPoint& pt2, double dShelfOffset = 10.0);
	CBCGPDiagramShelfConnector(const CBCGPDiagramShelfConnector& src);
	virtual ~CBCGPDiagramShelfConnector ();

// Operations:
public:
	void SetShelfSize (double dSize);
	double GetShelfSize () const;

// Overrides:
public:
	virtual CBCGPBaseVisualObject* CreateCopy () const;
	virtual UINT HitTestAnchorPoint (const CBCGPPoint& pt, int& nIndex) const;

// Implementation:
protected:
	virtual void SetTrackingRect(const CBCGPRect& rect);
	virtual void RecalcPoints();

protected:
	double	m_dShelfOffset;
};

//////////////////////////////////////////////////////////////////////
// CBCGPDiagramElbowConnector

class BCGCBPRODLLEXPORT CBCGPDiagramElbowConnector : public CBCGPDiagramConnector
{
	DECLARE_DYNAMIC(CBCGPDiagramElbowConnector)

	friend class CBCGPVisualConstructor;

public:
	enum Orientation
	{
		Auto = 0,	// Width >= Heght -- Horizontal, otherwise -- Vertical
		Horizontal,
		Vertical
	};

	CBCGPDiagramElbowConnector(Orientation orientation = Auto);
	CBCGPDiagramElbowConnector(const CBCGPRect& rect, Orientation orientation = Auto);
	CBCGPDiagramElbowConnector(const CBCGPPoint& pt1, const CBCGPPoint& pt2, const CBCGPPoint& ptResizeHandle = CBCGPPoint(), Orientation orientation = Auto);
	CBCGPDiagramElbowConnector(const CBCGPDiagramElbowConnector& src);

// Operations:
public:
	void SetResizeHandlePoint (const CBCGPPoint& ptResizeHandle);
	const CBCGPPoint& GetResizeHandlePoint () const;

	void SetOrientation (Orientation orientation);
	Orientation GetOrientation () const;

// Overrides:
public:
	virtual CBCGPBaseVisualObject* CreateCopy () const;
	virtual UINT HitTestAnchorPoint (const CBCGPPoint& pt, int& nIndex) const;

// Implementation:
protected:
	virtual void SetRect(const CBCGPRect& rect, BOOL bRedraw = FALSE);
	virtual void SetTrackingRect(const CBCGPRect& rect);
	virtual void RecalcPoints();
	virtual void OnEndTrackingPoints (BOOL bSaveChanges = TRUE);

protected:
	Orientation	m_Orientation;
	CBCGPPoint	m_ptResizeHandle;
	CBCGPPoint	m_ptTrackResizeHandle;
};

#endif // !defined(AFX_BCGPDIAGRAMCONNECTOR_H__D398297F_D298_496B_B7CF_25A1BE568972__INCLUDED_)
