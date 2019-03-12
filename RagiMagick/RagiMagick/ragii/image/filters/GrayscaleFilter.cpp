#include "GrayscaleFilter.h"

using namespace ragii::image;

void GrayscaleFilter::apply()
{
	int w = m_Params.width;
	int h = m_Params.height;
	int d = 24 / 8; // TODO:
	uint8_t *img = m_Params.image;

	const int pixelCount = w * h * d;

	int i = pixelCount;
	while (1)
	{

		int d = 0;
		d = img[0] * 77;
		d += img[1] * 150;
		d += img[2] * 29;

		uint8_t gray = uint8_t(d >> 8);

		img[0] = img[1] = img[2] = gray;

		img += sizeof(uint8_t) * d;
		i -= d;
		if (!i)
		{
			break;
		}
	}

	/**
	__asm
	{
		mov ebx, img
		mov ecx, pixelCount

	loop0 :
		mov dx, 0

		mov ax, 77			// 0.298912 * 256
		mul[ebx + 0]
		mov dx, ax

		mov ax, 150			// 0.586611 * 256
		mul[ebx + 1]
		add dx, ax

		mov ax, 29			// 0.114478 * 256
		mul[ebx + 2]
		add dx, ax

		shr dx, 8

		mov[ebx + 0], dl
		mov[ebx + 1], dl
		mov[ebx + 2], dl

		add ebx, d
		sub ecx, d
		jnz loop0
	}
	**/
}
