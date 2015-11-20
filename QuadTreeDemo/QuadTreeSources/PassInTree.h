#pragma once
#include "IQuadInterfaces.h"
#include <memory>

enum class EQTSide { Left = 0, Top, Right, Bottom };

struct SChildOrder
{
	byte    m_btCount;
	byte	m_btChildIndex[4];

	SChildOrder(byte in_btCount, byte in_btInd1, byte in_btInd2, byte in_btInd3, byte in_btInd4)
	{
		m_btCount = in_btCount;
		m_btChildIndex[0] = in_btInd1;
		m_btChildIndex[1] = in_btInd2;
		m_btChildIndex[2] = in_btInd3;
		m_btChildIndex[3] = in_btInd4;
	}
};

struct SPassInTree
{
	byte _maxtreedepth;
	IQuadTreeObject* _pobject;
	Rect2Dus m_Bound;
	byte m_btDepth;
	byte m_btMaxDepth;

	static const SChildOrder OctToChild[3][3];

	SPassInTree(const SPassInTree& pass) : 
		_maxtreedepth(pass._maxtreedepth),
		_pobject(pass._pobject), 
		m_Bound(pass.m_Bound), 
		m_btDepth(pass.m_btDepth), 
		m_btMaxDepth(pass.m_btMaxDepth)
	{}

	SPassInTree(byte inMaxTreeDepth, IQuadTreeObject* inpObject, Rect2Dus inBound, byte inDepth, byte inMaxDepth):
		_maxtreedepth(inMaxTreeDepth),
		_pobject(inpObject),
		m_Bound(inBound),
		m_btDepth(inDepth),
		m_btMaxDepth(inMaxDepth)
	{}

	SChildOrder	GetChildOrder()
	{
		int sh = _maxtreedepth - m_btDepth;
		int btCrdH1 = m_Bound.GetLeft() >> sh & 1;
		int btCrdH2 = m_Bound.GetRight() >> sh & 1;
		btCrdH1 += btCrdH2;

		int btCrdV1 = m_Bound.GetTop() >> sh & 1;
		int btCrdV2 = m_Bound.GetBottom() >> sh & 1;
		btCrdV1 += btCrdV2;

		return OctToChild[btCrdH1][btCrdV1];
	}

	bool IsTestIntersect() const { return _pobject != nullptr; }
	bool IsOnDepth() const 	{ return m_btDepth >= m_btMaxDepth; }
};