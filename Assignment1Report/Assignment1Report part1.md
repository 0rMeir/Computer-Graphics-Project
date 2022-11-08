

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	glm::ivec2 start = glm::ivec2(p1);
	glm::ivec2 end = glm::ivec2(p2);
	if (p1.x > p2.x)
	{
		start = glm::ivec2(p2);
		end = glm::ivec2(p1);
	}
	int transform1 = 1, transform2 = 1;
	int* coordinate1 = &start.y;
	int* coordinate2 = &start.x;
	int firstDelta = (end.x - start.x);
	int secondDelta = (end.y - start.y);

	double m = ((double)secondDelta / (double)firstDelta);

	int theEnd = end.x;
	int* theStart = &start.x;
	int firstDeltaChange = 1;
	int secondDeltaChange = 1;
	int e = 0;

	if(m < -1 || m > 1)
	{
		coordinate1 = &start.x;
		coordinate2 = &start.y;
		theEnd = end.y;
		theStart = &start.y;
		swap(firstDelta, secondDelta);

		if (m < -1)
		{
			transform2 = -1;
			firstDeltaChange = -1;
		}
	}
	if (m<=0 && m>=-1)
	{
		transform1 = -1;
		secondDeltaChange = -1;
	}

	while (*theStart != theEnd)
	{
		if (e > 0)
		{
			*coordinate1 += transform1;
			e -= 2 * (firstDelta)*firstDeltaChange;
		}
		PutPixel(start.x, start.y, color);
		*coordinate2 += transform2;	
		e += 2*(secondDelta)*secondDeltaChange;	
	}
}


![sanityCheck](https://user-images.githubusercontent.com/100144150/200578593-d49ca734-5c32-45e5-9056-199eb8c3d85b.JPG)




![Drawing](https://user-images.githubusercontent.com/100144150/200578682-415cadaa-b5b6-47c4-9ecd-4ee97d702a8c.JPG)


