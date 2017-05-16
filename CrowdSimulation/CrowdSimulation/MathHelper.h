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
	Eigen::MatrixXcd EigenVectors = EigenSolver.eigenvectors();//��ȡ������������  c��ʾ���Ӿ���
	Eigen::MatrixXcd EigenValues  = EigenSolver.eigenvalues();//��ȡ��������ֵ

	Eigen::MatrixXd EigenValueReal = EigenValues.real();//��ȡ����ֵ��ʵ������  ע������ûc

	Eigen::MatrixXf::Index EigenValuesMax;
	EigenValueReal.rowwise().sum().maxCoeff(&EigenValuesMax);//�õ��������ֵ��λ��

	Eigen::VectorXd EigenVector(CovMat.cols());
	for (int i=0; i<CovMat.cols(); ++i)
	{
		EigenVector(i) = EigenVectors.real()(i, EigenValuesMax);
	}

	return EigenVector;
}