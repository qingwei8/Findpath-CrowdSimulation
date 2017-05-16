#pragma once
#include <Eigen/dense>
#include <glm/glm.hpp>
#include <crtdbg.h>

//********************************************************************
//FUNCTION:
Eigen::MatrixXd computeCovarianceMatrix(const Eigen::MatrixXd& vInMatrix)
{
	Eigen::MatrixXd MeanVec = vInMatrix.colwise().mean();
	Eigen::RowVectorXd MeanVecRow(Eigen::RowVectorXd::Map(MeanVec.data(), vInMatrix.cols()));

	Eigen::MatrixXd ZeroMeanMat = vInMatrix;
	ZeroMeanMat.rowwise() -= MeanVecRow;

	Eigen::MatrixXd CovMat;
	if (vInMatrix.rows() == 1)
	{
		CovMat = (ZeroMeanMat.adjoint() * ZeroMeanMat) / double(vInMatrix.rows());
	}
	else
	{
		CovMat = (ZeroMeanMat.adjoint() * ZeroMeanMat) / double(vInMatrix.rows() - 1);
	}

	return CovMat;
}

//********************************************************************
//FUNCTION:
Eigen::VectorXd computeEigenVector(const Eigen::MatrixXd& vInfluenceMatrix)
{
	Eigen::MatrixXd CovMat = computeCovarianceMatrix(vInfluenceMatrix);
	_ASSERT(CovMat.rows() == CovMat.cols());

	Eigen::EigenSolver<Eigen::MatrixXd> EigenSolver(CovMat);
	Eigen::MatrixXcd EigenVectors = EigenSolver.eigenvectors();//获取矩阵特征向量  c表示复杂矩阵
	Eigen::MatrixXcd EigenValues  = EigenSolver.eigenvalues();//获取矩阵特征值

	Eigen::MatrixXd EigenValueReal = EigenValues.real();//获取特征值的实数部分  注意这里没c

	Eigen::MatrixXf::Index EigenValuesMax;
	EigenValueReal.rowwise().sum().maxCoeff(&EigenValuesMax);//得到最大特征值的位置

	Eigen::VectorXd EigenVector(CovMat.cols());
	for (int i=0; i<CovMat.cols(); ++i)
	{
		EigenVector(i) = EigenVectors.real()(i, EigenValuesMax);
	}

	return EigenVector;
}