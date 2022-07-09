#ifndef PHYSICS_DATA_H
#define PHYSICS_DATA_H


struct Vector3
{
    float x {0.0}, y {0.0}, z {0.0};

    void Update(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3& operator+(const Vector3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }


    Vector3& operator-(const Vector3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
};

#endif // PHYSICS_DATA_H