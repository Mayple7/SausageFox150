/*****************************************************************************/
/*!
\file				BoundingBox.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_BOUNDINGBOX
#define FOX_BOUNDINGBOX

void CreateBoundingBoxes(void);
void BoundingBoxUpdate(void);
void CreateBlockerBoxes(void);
void UpdateBlockerBoxes(float panelsize);

#endif
