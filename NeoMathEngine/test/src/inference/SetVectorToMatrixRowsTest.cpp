/* Copyright © 2017-2024 ABBYY

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
--------------------------------------------------------------------------------------------------------------*/

#include <TestFixture.h>

using namespace NeoML;
using namespace NeoMLTest;

static void setVectorToMatrixRowsTestImpl(const CTestParams& params, int seed)
{
	CRandom random(seed);

	const CInterval matrixHeightInterval = params.GetInterval("MatrixHeight");
	const CInterval matrixWidthInterval = params.GetInterval("MatrixWidth");
	const CInterval valuesInterval = params.GetInterval("Values");

	const int matrixHeight = random.UniformInt(matrixHeightInterval.Begin, matrixHeightInterval.End);
	const int matrixWidth = random.UniformInt(matrixWidthInterval.Begin, matrixWidthInterval.End);

	CREATE_FILL_FLOAT_ARRAY(vector, valuesInterval.Begin, valuesInterval.End, matrixWidth, random)

	std::vector<float> result;
	result.resize(matrixHeight * matrixWidth);
	MathEngine().SetVectorToMatrixRows(CARRAY_FLOAT_WRAPPER(result), matrixHeight, matrixWidth, CARRAY_FLOAT_WRAPPER(vector));

	for (int i = 0; i < matrixHeight; ++i) {
		for (int j = 0; j < matrixWidth; ++j) {
			EXPECT_NEAR( vector[j], result[i * matrixWidth + j], 1e-3 );
		}
	}
}

//------------------------------------------------------------------------------------------------------------

class CMathEngineSetVectorToMatrixRowsTest : public CTestFixtureWithParams {
};

TEST_P(CMathEngineSetVectorToMatrixRowsTest, Inference_SetVectorToMatrixRows)
{
	const auto met = MathEngine().GetType();
	if(met != MET_Cpu && met != MET_Cuda) {
		NEOML_HILIGHT( GTEST_LOG_( INFO ) ) << "Skip for MathEngine type= " << met << " , investigate later.\n";
		return;
	}

	RUN_TEST_IMPL( setVectorToMatrixRowsTestImpl );
}

INSTANTIATE_TEST_CASE_P(CMathEngineSetVectorToMatrixRowsTestInstantiation, CMathEngineSetVectorToMatrixRowsTest,
	::testing::Values(
		CTestParams(
			"MatrixHeight = (1..100);"
			"MatrixWidth = (1..100);"
			"Values = (-50..50);"
			"TestCount = 100;"
		),
		CTestParams(
			"MatrixHeight = (1089536..1089536);"
			"MatrixWidth = (48..48);"
			"Values = (-1..1);"
			"TestCount = 1;"
		),
		CTestParams(
			"MatrixHeight = (1089536..1089536);"
			"MatrixWidth = (64..64);"
			"Values = (-1..1);"
			"TestCount = 1;"
		)
	)
);
