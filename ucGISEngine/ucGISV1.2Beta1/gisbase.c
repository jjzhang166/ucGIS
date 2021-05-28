#define GISBASE_GLOBALES
#include "gisbase.h"

//¶ÁÈ¡»ù±¾µÄ±ê×¢
BOOLEAN Lable_ReadFromFile(FILE *fp,GBaseLabel *lpLabel)
{
	INT32U gblen=0;
	INT8U nLableLen = 0;
	GISTCHAR pszLabel[100];
	GISCHAR szLabel[100];
	GISCHAR* ptext;
	fread(&nLableLen,1,sizeof(INT8U),fp);
	pszLabel[nLableLen] = '\0';
	if(nLableLen>0)
	{
		fread(pszLabel,sizeof(GISTCHAR),nLableLen,fp);
        
		gblen=WToA(pszLabel,szLabel,100);
		szLabel[gblen]='\0';
		lpLabel->text = GUI_ALLOC_AllocZero(gblen+1);
		if (!lpLabel->text) 
		{
   		  //MessageBox(NULL,_T("ÉêÇëÄÚ´æÊ§°Ü£¡"),_T("´íÎó"),MB_ICONERROR);
		  GUI_DEBUG_ERROROUT("ÉêÇëÄÚ´æÊ§°Ü£¡");
			return FALSE;
		}
		else
		{
      
 		  WM_LOCK();
          ptext=(GISCHAR*) GUI_ALLOC_h2p(lpLabel->text);
		  memset(ptext,0,gblen+1);
		  strcpy(ptext, szLabel);
		  WM_UNLOCK();
		}
	}
    else
	{
/*
		szLabel[0]=' ';
		szLabel[1]=' ';
		szLabel[2]='\0';


		lpLabel->text = GUI_ALLOC_AllocZero(3);
		if (!lpLabel->text) 
		{
   		  //MessageBox(NULL,_T("ÉêÇëÄÚ´æÊ§°Ü£¡"),_T("´íÎó"),MB_ICONERROR);
		  GUI_DEBUG_ERROROUT("ÉêÇëÄÚ´æÊ§°Ü£¡");
			return FALSE;
		}
		else
		{
      
 		  WM_LOCK();
          ptext=(GISCHAR*) GUI_ALLOC_h2p(lpLabel->text);
		  memset(ptext,0,3);
		  strcpy(ptext, szLabel);
		  WM_UNLOCK();
		}
*/
	}
	fread(&lpLabel->m_nLayer,1,sizeof(lpLabel->m_nLayer),fp);
	fread(&lpLabel->m_Rect,1,sizeof(lpLabel->m_Rect),fp);
	fread(&lpLabel->m_nType,1,sizeof(lpLabel->m_nType),fp);
	fread(&lpLabel->m_nSubType,1,sizeof(lpLabel->m_nSubType),fp);

	//memset(m_lpszLabel,0,nLableLen);
	//lstrcpy(m_lpszLabel,pszLabel);
    return TRUE;
}

/*********************************************************************
*
*       GIS_IntersectRects
*
* Purpose:
*   Calc intersection of rectangles
*
* Return value:
*   1 if they do intersect,
*   0 if there is no intersection
*
* Add. info:
*   Rectangles are passed as pointers. These pointers need to be valid;
*   a NULL pointer may not be passed. There is no check for NULL pointers
*   implemented in order to avoid avoid performance penalty.
*/

INT32S GIS_IntersectRects(GIS_RECT* pDest, const GIS_RECT* pr0, const GIS_RECT* pr1) {
  pDest->left = GISMAX (pr0->left, pr1->left);
  pDest->top = GISMAX (pr0->top, pr1->top);
  pDest->right = GISMIN (pr0->right, pr1->right);
  pDest->bottom = GISMIN (pr0->bottom, pr1->bottom);
  if (pDest->right < pDest->left) {
    return 0;
  }
  if (pDest->bottom < pDest->top) {
    return 0;
  }
  return 1;
}
/*
void GUI_GetHZFontLen(INT16U hz,INT32U *cx,INT32U *cy)
{


}

*/
GISBASE_EXT void Label_Free(GBaseLabel *pgBaseLabel)
{
   	GUI_ALLOC_Free(pgBaseLabel->text);
	memset(pgBaseLabel,0,sizeof(GBaseLabel));
}
int FieldVar(const char *buf, int fldnum, int sep, char *dest)
{
	int i;
	const char *ptr = buf;
	
	for (i=0; i < fldnum; i++)
	{
		if ((ptr = (char *) strchr(ptr,sep)))
			ptr++;
		else
			return 0;
	}
	
	for (i=0; (*ptr != sep) && (*ptr) && (i < 4096); i++)
		*dest++ = *ptr++;
	*dest = '\0';
	
	if (*ptr != sep)
		return 0;
	
	return i;
}
/*
BOOLEAN mapPtInRect( mapMAPRECTSTRCT* r,GeoPoint* point)
{
	return (point->lon>r->fMinX)&&(point->lat<r->fMinY)&&(point->lon<r->fMaxX)&&(point->lat>r->fMaxY);
}
*/
/*************************************************************************

* FUNCTION:   G_PtInPolygon
*
* PURPOSE
* This routine determines if the mapMAPPOINTSTRUCT passed is in the polygon. It uses

* the classical polygon hit-testing algorithm: a horizontal ray starting

* at the mapMAPPOINTSTRUCT is extended infinitely rightwards and the number of
* polygon edges that intersect the ray are counted. If the number is odd,

* the mapMAPPOINTSTRUCT is inside the polygon.
*
* RETURN VALUE
* (BOOL) TRUE if the mapMAPPOINTSTRUCT is inside the polygon, FALSE if not.
*************************************************************************/ 

/*
BOOLEAN G_PtInPolygon(GeoPoint *rgpts, INT16U wnumpts, GeoPoint ptTest,mapRECT *prbound)
{

	mapRECT   r ;
	GeoPoint  *ppt ;
	INT16U   i ;
	GeoPoint  pt1, pt2 ;
	INT16U   wnumintsct = 0 ;

	memset(&r,0,sizeof(r));

	if (!G_PtInPolyRect(rgpts,wnumpts,ptTest,prbound))

		return FALSE ;

	pt1 = pt2 = ptTest ;
	pt1.lon=pt2.lon=ptTest.lon;
	pt1.lat=pt2.lat=ptTest.lat;
	pt2.lon = r.right + 50 ;

	// Now go through each of the lines in the polygon and see if it
	// intersects
	for (i = 0, ppt = rgpts ; i < wnumpts-1 ; i++, ppt++)
	{
		if (Intersect(ptTest, pt2, *ppt, *(ppt+1)))
			wnumintsct++ ;
	}

	// And the last line
	if (Intersect(ptTest, pt2, *ppt, *rgpts))
		wnumintsct++ ;

	return (wnumintsct&1) ;

}
*/
/*************************************************************************

* FUNCTION:   G_PtInPolyRect
*
* PURPOSE
* This routine determines if a mapMAPPOINTSTRUCT is within the smallest rectangle
* that encloses a polygon.
*
* RETURN VALUE
* (BOOL) TRUE or FALSE depending on whether the mapMAPPOINTSTRUCT is in the rect or

* not.
*************************************************************************/ 

/*
BOOL  G_PtInPolyRect(GeoPoint *rgpts, WORD wnumpts, GeoPoint ptTest,

					 mapRECT *prbound)

{

	mapRECT r ;
	// If a bounding rect has not been passed in, calculate it
	if (prbound)

		r = *prbound ;

	else
	{

		float   xmin, xmax, ymin, ymax ;
		mapMAPPOINTSTRUCT *ppt ;
		WORD  i ;

		xmin = xmax = rgpts[0].x ;
		ymin = ymax = rgpts[0].y ;

		for (i=0, ppt = rgpts ; i < wnumpts ; i++, ppt++)
		{
			if (ppt->x < xmin)
				xmin = ppt->x ;
			if (ppt->x > xmax)
				xmax = ppt->x ;
			if (ppt->y < ymin)
				ymin = ppt->y ;
			if (ppt->y > ymax)
				ymax = ppt->y ;
		}
		r.left = xmin;
		r.top = ymax;
		r.right = xmax;
		r.bottom = ymin;
//		SetRect(&r, xmin, ymin, xmax, ymax) ;  //?????

	}
	return mapPtInRect(&r,&ptTest);

}
*/
/*************************************************************************
* FUNCTION:   Intersect
*
* PURPOSE
* Given two line segments, determine if they intersect.
*
* RETURN VALUE
* TRUE if they intersect, FALSE if not.
*************************************************************************/ 
/*
BOOLEAN Intersect(GeoPoint p1, GeoPoint p2, GeoPoint p3, GeoPoint p4)
{

	return ((( CCW(p1, p2, p3) * CCW(p1, p2, p4)) <= 0)

		&& (( CCW(p3, p4, p1) * CCW(p3, p4, p2)  <= 0) )) ;

}
*/
/*************************************************************************

* FUNCTION:   CCW (CounterClockWise)
*
* PURPOSE
* Determines, given three mapMAPPOINTSTRUCTs, if when travelling from the first to
* the second to the third, we travel in a counterclockwise direction.
*
* RETURN VALUE
* (int) 1 if the movement is in a counterclockwise direction, -1 if
* not.
*************************************************************************/ 
/*
int CCW(GeoPoint p0, GeoPoint p1, GeoPoint p2)

{

	FP32 dx1, dx2 ;
	FP32 dy1, dy2 ;

	dx1 = p1.x - p0.x ; dx2 = p2.x - p0.x ;
	dy1 = p1.y - p0.y ; dy2 = p2.y - p0.y ;

	// This is basically a slope comparison: we don't do divisions because

	//of divide by zero possibilities with pure horizontal and pure
	//vertical lines.
	// 

	return ((dx1 * dy2 > dy1 * dx2) ? 1 : -1) ;

}
*/