#pragma once
#include "OBB2D.h"
#include "Circle2D.h"
#include "Rect2D.h"
#include "Segment2D.h"

bool TestIntersectionBoxSphere(const OBB2Df& Box, const Circle2Df& Circle);
bool TestIntersectionBoxSphere(const Rect2Df& BoundBox, const Circle2Df& Circle);
bool TestIntersectionBoxBox(const OBB2Df& Box0, const OBB2Df& Box1);


bool IsIntersectSegment(const Segment2Df& inSegment, float inT, const OBB2Df& inBox, int& out_iQuantity, float out_fT[2], Vector2Df out_vPoint[2], Vector2Df out_vNormal[2]);
bool IsIntersectSegment(const Segment2Df& in_Segment, float inT, const Circle2Df& inCircle, int& out_iQuantity, float out_fT[2], Vector2D<float> out_vPoint[2], Vector2D<float> out_vNormal[2]);

