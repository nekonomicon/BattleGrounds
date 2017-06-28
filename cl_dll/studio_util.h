/*
    Copyright 2001 to 2004. The Battle Grounds Team and Contributors

    This file is part of the Battle Grounds Modification for Half-Life.

    The Battle Grounds Modification for Half-Life is free software;
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser General Public License as published by the Free
    Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    The Battle Grounds Modification for Half-Life is distributed in
    the hope that it will be useful, but WITHOUT ANY WARRANTY; without
    even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE.  See the GNU Lesser General Public License
    for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with The Battle Grounds Modification for Half-Life;
    if not, write to the Free Software Foundation, Inc., 59 Temple Place,
    Suite 330, Boston, MA  02111-1307  USA

    You must obey the GNU Lesser General Public License in all respects for
    all of the code used other than code distributed with the Half-Life
    SDK developed by Valve.  If you modify this file, you may extend this
    exception to your version of the file, but you are not obligated to do so.
    If you do not wish to do so, delete this exception statement from your
    version.
*/

//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============

#if !defined( STUDIO_UTIL_H )
#define STUDIO_UTIL_H
#if defined( WIN32 )
#pragma once
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

#ifndef PITCH
// MOVEMENT INFO
// up / down
#define	PITCH	0
// left / right
#define	YAW		1
// fall over
#define	ROLL	2
#endif

#define FDotProduct( a, b ) (fabs((a[0])*(b[0])) + fabs((a[1])*(b[1])) + fabs((a[2])*(b[2])))
void	AngleMatrix (const float *angles, float (*matrix)[4] );
int		VectorCompare (const float *v1, const float *v2);
void	CrossProduct (const float *v1, const float *v2, float *cross);
void	VectorTransform (const float *in1, float in2[3][4], float *out);
void	ConcatTransforms (float in1[3][4], float in2[3][4], float out[3][4]);
void	MatrixCopy( float in[3][4], float out[3][4] );
void	QuaternionMatrix( vec4_t quaternion, float (*matrix)[4] );
void	QuaternionSlerp( vec4_t p, vec4_t q, float t, vec4_t qt );
void	AngleQuaternion( float *angles, vec4_t quaternion );

#endif // STUDIO_UTIL_H