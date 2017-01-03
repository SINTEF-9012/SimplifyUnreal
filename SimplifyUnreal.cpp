#include "SimplifyUnreal.h"

float getSqSegDist(FVector2D& p, FVector2D& p1, FVector2D& p2) {
	float x = p1.X;
	float y = p1.Y;
	float dx = p2.X - x;
	float dy = p2.Y - y;

	if (dx != 0.0f || dy != 0.0f) {
		float t = ((p.X - x) * dx + (p.Y - y) * dy) / (dx * dx + dy * dy);

		if (t > 1.0f) {
			x = p2.X;
			y = p2.Y;

		}
		else if (t > 0.0f) {
			x += dx * t;
			y += dy * t;
		}
	}

	dx = p.X - x;
	dy = p.Y - y;

	return dx * dx + dy * dy;
}

TArray<FVector2D> simplifyRadialDist(TArray<FVector2D>& points, float sqTolerance) {
	FVector2D prevPoint = points[0];
	TArray<FVector2D> newPoints;
	newPoints.Push(prevPoint);
	FVector2D point;

	for (int i = 1; i < points.Num(); ++i) {
		point = points[i];
		if (FVector2D::DistSquared(point, prevPoint) > sqTolerance) {
			newPoints.Push(point);
			prevPoint = point;
		}
	}

	if (prevPoint != point) newPoints.Push(point);

	return newPoints;
}

void simplifyDPStep(TArray<FVector2D>& points, int first, int last, float sqTolerance, TArray<FVector2D>& simplified) {
	float maxSqDist = sqTolerance;
	int index = -1;

	for (int i = first + 1; i < last; ++i) {
		float sqDist = getSqSegDist(points[i], points[first], points[last]);

		if (sqDist > maxSqDist) {
			index = i;
			maxSqDist = sqDist;
		}
	}

	if (maxSqDist > sqTolerance) {
		if (index - first > 1) simplifyDPStep(points, first, index, sqTolerance, simplified);
		simplified.Add(points[index]);
		if (last - index > 1) simplifyDPStep(points, index, last, sqTolerance, simplified);
	}
}

TArray<FVector2D> simplifyDouglasPeucker(TArray<FVector2D>& points, float sqTolerance) {
	int last = points.Num() - 1;

	TArray<FVector2D> simplified;
	simplified.Add(points[0]);
	simplifyDPStep(points, 0, last, sqTolerance, simplified);
	simplified.Add(points[last]);

	return simplified;
}

TArray<FVector2D> USimplifyUnreal::Simplify(TArray<FVector2D> points, float tolerance /*= 1.0f*/, bool highestQuality/* = false*/) {

	if (points.Num() <= 2) return points;

	float sqTolerance = tolerance * tolerance;

	TArray<FVector2D> simplifiedPoints = simplifyDouglasPeucker(points, sqTolerance);

	return simplifiedPoints;
}
