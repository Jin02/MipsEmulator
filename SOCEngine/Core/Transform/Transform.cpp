#include "Transform.h"

using namespace Math;

namespace Core
{
	Transform::Transform(Transform *parent)
	{
		_parent = parent;

		if(parent)
			_root = parent->_root;
		else _root = this;

		_forward = Vector3(0.0f, 0.0f, 1.0f);
		_right	 = Vector3(1.0f, 0.0f, 0.0f);
		_up	 	 = Vector3(0.0f, 1.0f, 0.0f);

		_localPosition	  = Vector3(0.0f, 0.0f, 0.0f);
		_rotation		  = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		_localScale		  = Vector3(1.0f, 1.0f, 1.0f);
		_localEulerAngle = Vector3(0.0f, 0.0f, 0.0f);

		for(Transform *o = this; o != NULL; o = o->_parent)
		{
			_position += o->_localPosition;

			_scale.x *= o->_localScale.x;
			_scale.y *= o->_localScale.y;
			_scale.z *= o->_localScale.z;

			_eulerAngles += _localEulerAngle;
		}

		_radius = 0.0f; // default
	}

	Transform::~Transform(void)
	{

	}

	void Transform::LookAt(const Vector3& worldPosition)
	{
		UpdateWorldTransform();

		Vector3 dir = worldPosition - _position;
		dir.Normalize();

		_forward = dir;
		_right = Vector3::Cross(Vector3::Up(), dir);
		_up = Vector3::Cross(dir, _right);
		_right.Normalize();
		_up.Normalize();

		Matrix rotationMatrix;

		rotationMatrix._m[0][0] = _right.x;
		rotationMatrix._m[1][0] = _right.y;
		rotationMatrix._m[2][0] = _right.z;
		rotationMatrix._m[3][0] = 0.0f;

		rotationMatrix._m[0][1] = _up.x;
		rotationMatrix._m[1][1] = _up.y;
		rotationMatrix._m[2][1] = _up.z;
		rotationMatrix._m[3][1] = 0.0f;

		rotationMatrix._m[0][2] = _forward.x;
		rotationMatrix._m[1][2] = _forward.y;
		rotationMatrix._m[2][2] = _forward.z;
		rotationMatrix._m[3][2] = 0.0f;

		rotationMatrix._m[0][3] = 0;
		rotationMatrix._m[1][3] = 0;
		rotationMatrix._m[2][3] = 0;
		rotationMatrix._m[3][3] = 1.0f;

		Quaternion::RotationMatrix(_rotation, rotationMatrix);

		_localEulerAngle.y = Math::Common::Rad2Deg( atan2(-rotationMatrix._13, rotationMatrix._11) );
		_localEulerAngle.x = Math::Common::Rad2Deg( atan2(-rotationMatrix._33, rotationMatrix._22) );
		_localEulerAngle.z = Math::Common::Rad2Deg( asin(rotationMatrix._12) );

		Math::Common::EulerNormalize(_localEulerAngle, _localEulerAngle);

		UpdateMatrix();
	}

	void Transform::LookAt(Transform *target)
	{
		target->UpdateWorldTransform();
		LookAt(target->_position);
	}

	void Transform::Rotate(const Vector3& eulerAngles)
	{
		Vector3 euler = _localEulerAngle + eulerAngles;
		UpdateEulerAngles(euler);
	}

	void Transform::Rotate(float x, float y, float z)
	{
		Vector3 euler = _localEulerAngle + Vector3(x, y, z);
		UpdateEulerAngles(euler);
	}

	void Transform::Translate(const Vector3& translation)
	{
		Vector3 p = _localPosition + translation;
		UpdateEulerAngles(p);
	}

	void Transform::TranslateWithUpVec(float units)
	{
		_localPosition += _up * units;
	}

	void Transform::TranslateWithForwardVec(float units)
	{
		_localPosition += _forward * units;
	}

	void Transform::TranslateWithRightVec(float units)
	{
		_localPosition += _right * units;
	}

	void Transform::WorldMatrix(Math::Matrix& outMatrix)
	{
		Matrix mat;
		mat.Identity();

		UpdateMatrix();

		for(Transform *o = this; o != NULL; o = o->_parent)
			mat *= o->_matrix;

		outMatrix = mat;
	}

	void Transform::UpdatePosition(const Math::Vector3& position)
	{
		_localPosition = position;
		UpdateMatrix();
	}

	void Transform::UpdateRotation(const Math::Quaternion& quaternion)
	{
		_rotation = quaternion; 

		Matrix rotationMatrix;
		Matrix::RotationQuaternion(rotationMatrix, quaternion);

		_right	 = Vector3(rotationMatrix._11, rotationMatrix._21, rotationMatrix._31);
		_up		 = Vector3(rotationMatrix._12, rotationMatrix._22, rotationMatrix._32);
		_forward = Vector3(rotationMatrix._13, rotationMatrix._23, rotationMatrix._33);

		Vector3 euler;
		Math::Quaternion::ToEuler(euler, _rotation);

		_localEulerAngle.x = Math::Common::Rad2Deg( euler.x );
		_localEulerAngle.y = Math::Common::Rad2Deg( euler.y );
		_localEulerAngle.z = Math::Common::Rad2Deg( euler.z );

		UpdateMatrix();
	}

	void Transform::UpdateScale(const Vector3& scale)
	{
		_localScale = scale;
		UpdateMatrix();
	}

	void Transform::UpdateEulerAngles(const Vector3& euler)
	{
		Math::Common::EulerNormalize(_localEulerAngle, euler);

		Vector3 re;
		re.x = Math::Common::Deg2Rad( euler.x );
		re.y = Math::Common::Deg2Rad( euler.y );
		re.z = Math::Common::Deg2Rad( euler.z );

		Matrix rotationMatrix;
		Quaternion::RotationYawPitchRoll(_rotation, re.y, re.x, re.z);
		Matrix::RotationQuaternion(rotationMatrix, _rotation);

		_right		= Vector3(rotationMatrix._m[0][0], rotationMatrix._m[1][0], rotationMatrix._m[2][0]);
		_up 		= Vector3(rotationMatrix._m[0][1], rotationMatrix._m[1][1], rotationMatrix._m[2][1]);
		_forward	= Vector3(rotationMatrix._m[0][2], rotationMatrix._m[1][2], rotationMatrix._m[2][2]);

		UpdateMatrix();
	}

	void Transform::UpdateDirection(const Math::Vector3& dir)
	{
		Vector3 worldPos;
		WorldPosition(worldPos);

		Vector3 p = worldPos + dir;
		LookAt(p);
	}

	void Transform::UpdateWorldTransform()
	{
		Vector3 p(0, 0, 0), s(1, 1, 1), e(0, 0, 0);

		for(Transform *o = this; o != NULL; o = o->_parent)
		{
			p += o->_localPosition;

			s.x *= o->_localScale.x;
			s.y *= o->_localScale.y;
			s.z *= o->_localScale.z;

			e += _localEulerAngle;
		}

		 Math::Common::EulerNormalize(_eulerAngles, e);
		_position = p;
		_scale = s;
	}

	void Transform::UpdateMatrix()
	{
		Matrix::RotationQuaternion(_matrix, _rotation);

		_matrix._11 *= _localScale.x;
		_matrix._22 *= _localScale.y;
		_matrix._33 *= _localScale.z;

		_matrix._14 = _localPosition.x;
		_matrix._24 = _localPosition.y;
		_matrix._34 = _localPosition.z;
		_matrix._44 = 1.0f;
	}

	void Transform::WorldPosition(Math::Vector3& outPosition)
	{
		Vector3 p(0, 0, 0);

		for(Transform *o = this; o != NULL; o = o->_parent)
			p += o->_localPosition;

		outPosition = p;
	}

	void Transform::Billboard(Math::Matrix& outMatrix, const Math::Matrix& camWorldMat)
	{
		Matrix mat = camWorldMat;

		mat._11 = camWorldMat._11;
		mat._13 = camWorldMat._13;
		mat._31 = camWorldMat._31;
		mat._33 = camWorldMat._33;

		Matrix::Inverse(outMatrix, mat);
	}

	float Transform::CalcRadius(Transform *child)
	{
		Vector3 worldPosition;
		WorldPosition(worldPosition);

		Vector3 childWorldPosition;
		child->WorldPosition(childWorldPosition);

		float distance = Vector3::Distance(worldPosition, childWorldPosition);
		_radius = std::max(distance + child->_radius, _radius);

		return _radius;
	}

	void Transform::WorldEulerAngle(Math::Vector3& outEuler)
	{
		Vector3 p(0, 0, 0);

		for(Transform *o = this; o != NULL; o = o->_parent)
			p += o->_localEulerAngle;

		outEuler = p;
	}

	void Transform::WorldScale(Math::Vector3& outScale)
	{
		Vector3 p(1, 1, 1);

		for(Transform *o = this; o != NULL; o = o->_parent)
		{
			p.x *= o->_localScale.x;
			p.y *= o->_localScale.y;
			p.z *= o->_localScale.z;
		}

		outScale = p;
	}
}