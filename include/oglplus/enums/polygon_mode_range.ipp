/*
 *  .file oglplus/enums/polygon_mode_range.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/oglplus/polygon_mode.txt' instead.
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace enums {
OGLPLUS_LIB_FUNC aux::CastIterRange<
	const GLenum*,
	PolygonMode
> _ValueRange(PolygonMode*)
#if (!OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OGLPLUS_IMPL_EVR_POLYGONMODE)
#define OGLPLUS_IMPL_EVR_POLYGONMODE
{
static const GLenum _values[] = {
#if defined GL_POINT
GL_POINT,
#endif
#if defined GL_LINE
GL_LINE,
#endif
#if defined GL_FILL
GL_FILL,
#endif
0
};
return aux::CastIterRange<
	const GLenum*,
	PolygonMode
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

