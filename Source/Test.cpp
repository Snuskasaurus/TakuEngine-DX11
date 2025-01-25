#include "Test.h"

#include "Engine/Math.h"

void TestValidation::RunTest_Matrix()
{
	// Testing the matrix conversions
	{
	}
	
    // Testing MatrixTranslation
    {
        TVector3f Translation = { 10.0f, 5.0f, 3.0f };
		
        TMatrix4f matrix = TMatrix4f::MatrixTranslation(Translation);
        TMatrix4f directXMatrix = TMatrix4f::FromDirectXMatrix(DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z));
		
        assert(matrix.x == directXMatrix.x);
        assert(matrix.y == directXMatrix.y);
        assert(matrix.z == directXMatrix.z);
        assert(matrix.w == directXMatrix.w);
    }

    // Testing MatrixRotationX
    {
        float Rotation = 99.0f;
		
        TMatrix4f matrix = TMatrix4f::MatrixRotationX(Rotation);
        TMatrix4f directXMatrix = TMatrix4f::FromDirectXMatrix(DirectX::XMMatrixRotationX(Rotation));
		
        assert(matrix.x == directXMatrix.x);
        assert(matrix.y == directXMatrix.y);
        assert(matrix.z == directXMatrix.z);
        assert(matrix.w == directXMatrix.w);
    }

    // Testing MatrixRotationY
    {
        float Rotation = 187.0f;
		
        TMatrix4f matrix = TMatrix4f::MatrixRotationY(Rotation);
        TMatrix4f directXMatrix = TMatrix4f::FromDirectXMatrix(DirectX::XMMatrixRotationY(Rotation));
		
        assert(matrix.x == directXMatrix.x);
        assert(matrix.y == directXMatrix.y);
        assert(matrix.z == directXMatrix.z);
        assert(matrix.w == directXMatrix.w);
    }

    // Testing MatrixRotationZ
    {
        float Rotation = 166.0f;
		
        TMatrix4f MatrixRotationZ = TMatrix4f::MatrixRotationZ(Rotation);
        TMatrix4f XMMatrixRotationZ = TMatrix4f::FromDirectXMatrix(DirectX::XMMatrixRotationZ(Rotation));
		
        assert(MatrixRotationZ.x == XMMatrixRotationZ.x);
        assert(MatrixRotationZ.y == XMMatrixRotationZ.y);
        assert(MatrixRotationZ.z == XMMatrixRotationZ.z);
        assert(MatrixRotationZ.w == XMMatrixRotationZ.w);
    }

    // Testing MatrixScale
    {
        float scale = 166.0f;
		
        TMatrix4f matrix = TMatrix4f::MatrixScale(scale);
        TMatrix4f directXMatrix = TMatrix4f::FromDirectXMatrix(DirectX::XMMatrixScaling(scale, scale, scale));
		
        assert(matrix.x == directXMatrix.x);
        assert(matrix.y == directXMatrix.y);
        assert(matrix.z == directXMatrix.z);
        assert(matrix.w == directXMatrix.w);
    }

	// Testing matrix multiplication
	{
		TVector3f Translation = { 10.0f, 5.0f, 3.0f };
		float Rotation = 166.0f;
		float scale = 166.0f;

		TMatrix4f matrixT = TMatrix4f::MatrixTranslation(Translation);
		TMatrix4f matrixR = TMatrix4f::MatrixRotationY(Rotation);
		TMatrix4f matrixS = TMatrix4f::MatrixScale(scale);
        TMatrix4f matrix = matrixT * matrixR * matrixS;

		DirectX::XMMATRIX directXMatrixT = DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);
		DirectX::XMMATRIX directXMatrixR = DirectX::XMMatrixRotationY(Rotation);
		DirectX::XMMATRIX directXMatrixS = DirectX::XMMatrixScaling(scale, scale, scale);
		TMatrix4f directXMatrix = TMatrix4f::FromDirectXMatrix(directXMatrixT * directXMatrixR * directXMatrixS);
		
		assert(matrix.x == directXMatrix.x);
		assert(matrix.y == directXMatrix.y);
		assert(matrix.z == directXMatrix.z);
		assert(matrix.w == directXMatrix.w);
	}

	//// Testing MatrixLookAtRH
	//{
	//	const TVector3f cameraPosition = { 12.0f, 36.0f, 25.0f };
	//	const TVector3f lookAtPosition = { 0.0f, 100.0f, 1.0f };
	//	const TVector3f up = { 0.0f, 0.0f, 1.0f };
	//	TMatrix4f MatrixLookAtRH = TMatrix4f::MatrixLookAtRH(cameraPosition, lookAtPosition, up);
	//	DirectX::XMMATRIX matrixDirectX = DirectX::XMMatrixLookAtRH(cameraPosition.ToDXVec(), lookAtPosition.ToDXVec(), up.ToDXVec());

	//	TMatrix4f XMMatrixLookAtRH = TMatrix4f::Transpose(TMatrix4f::FromDirectXMatrix(matrixDirectX));
	//	assert(MatrixLookAtRH.x == XMMatrixLookAtRH.x);
	//	assert(MatrixLookAtRH.y == XMMatrixLookAtRH.y);
	//	assert(MatrixLookAtRH.z == XMMatrixLookAtRH.z);
	//	assert(MatrixLookAtRH.w == XMMatrixLookAtRH.w);
	//}
}
