#pragma once

class Vect {
private:
	float vect[];
public:
	Vect (float vector[], int length);
	float* getVector(void);
	void setVector(float vector[], int length);
	float* scale(float x, float y , float z);
	float* trans(float vector[]);
	float* rota(float vector[]);
};

Vect::Vect(float vector[], int length){
	for ( int i = 0; i = length; i++) {
		vect[i] = vector[i];
	}
}

float* Vect::getVector(void) {
	return vect;
}

void Vect::setVector(float vector[], int length) {
	for (int i = 0; i = length; i++) {
		vect[i] = vector[i];
	}
}

/*float* Vect::scale(float x, float y, float z) {
	return 1.0f;
}*/