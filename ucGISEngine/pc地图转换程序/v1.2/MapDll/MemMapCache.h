/*
内存地图缓存
用于在内存中绘制地图
该缓存包含一个地图预处理边界，当当前地图创建超出边界时，将触发临近地图的再绘制。
地图绘制边界
*/

#pragma once

class CMemMapCache
{
public:
	CMemMapCache(void);
	~CMemMapCache(void);
private:
	float m_nPreBorderHeight,m_nPreBorderWidth;


}; 
