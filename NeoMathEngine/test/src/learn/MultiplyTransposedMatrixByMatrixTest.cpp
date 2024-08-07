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

static void multiplyTransposedMatrixByMatrixNaive( int batchSize, const std::vector<float>& first, const std::vector<float>& second,
	int firstHeight, int firstWidth, int secondWidth, std::vector<float>& result )
{
	for( int b = 0; b < batchSize; ++b ) {
		const int firstStart = b * firstHeight * firstWidth;
		const int secondStart = b * firstHeight * secondWidth;
		const int resultStart = b * firstWidth * secondWidth;
		for( int i = 0; i < firstWidth; ++i ) {
			for( int j = 0; j < secondWidth; ++j ) {
				for( int k = 0; k < firstHeight; ++k ) {
					result[resultStart + i * secondWidth + j] += first[firstStart + k * firstWidth + i] * second[secondStart + k * secondWidth + j];
				}
			}
		}
	}
}

static void multiplyTransposedMatrixByMatrixTestImpl( const CTestParams& params, int seed )
{
	CRandom random( seed );

	const CInterval batchSizeInterval = params.GetInterval( "BatchSize" );
	const CInterval widthInterval = params.GetInterval( "Width" );
	const CInterval heightInterval = params.GetInterval( "Height" );
	const CInterval valuesInterval = params.GetInterval( "Values" );

	const int batchSize = random.UniformInt( batchSizeInterval.Begin, batchSizeInterval.End );
	const int firstHeight = random.UniformInt( heightInterval.Begin, heightInterval.End );
	const int firstWidth = random.UniformInt( widthInterval.Begin, widthInterval.End );
	const int secondWidth = random.UniformInt( widthInterval.Begin, widthInterval.End );

	CREATE_FILL_FLOAT_ARRAY( first, valuesInterval.Begin, valuesInterval.End, batchSize * firstHeight * firstWidth, random )
	CREATE_FILL_FLOAT_ARRAY( second, valuesInterval.Begin, valuesInterval.End, batchSize * firstHeight * secondWidth, random )

	std::vector<float> actual( batchSize * firstWidth * secondWidth, 0.f );
	std::vector<float> expected = actual;

	multiplyTransposedMatrixByMatrixNaive( batchSize, first, second, firstHeight, firstWidth, secondWidth, expected );

	MathEngine().MultiplyTransposedMatrixByMatrix( batchSize, CARRAY_FLOAT_WRAPPER( first ), firstHeight, firstWidth,
		CARRAY_FLOAT_WRAPPER( second ), secondWidth, CARRAY_FLOAT_WRAPPER( actual ), static_cast<int>( actual.size() ) );

	for( int i = 0; i < firstWidth * secondWidth; ++i ) {
		EXPECT_NEAR( expected[i], actual[i], 1e-3 ) << i;
	}
}

//---------------------------------------------------------------------------------------------------------------------

class CMultiplyTransposedMatrixByMatrixTest : public CTestFixtureWithParams {
};

INSTANTIATE_TEST_CASE_P( CMultiplyTransposedMatrixByMatrixTestInstantiation, CMultiplyTransposedMatrixByMatrixTest,
	::testing::Values(
		CTestParams(
			"Height = (1..50);"
			"Width = (1..50);"
			"BatchSize = (1..5);"
			"Values = (-1..1);"
			"TestCount = 100;"
		),
		CTestParams(
			"Height = (100..500);"
			"Width = (100..500);"
			"BatchSize = (1..5);"
			"Values = (-1..1);"
			"TestCount = 5;"
		)
	)
);

TEST_P( CMultiplyTransposedMatrixByMatrixTest, Random )
{
	const auto met = MathEngine().GetType();
	if(met != MET_Cpu && met != MET_Cuda) {
		NEOML_HILIGHT( GTEST_LOG_( INFO ) ) << "Skipped rest of test for MathEngine type=" << met << " because no implementation.\n";
		return;
	}

	RUN_TEST_IMPL( multiplyTransposedMatrixByMatrixTestImpl )
}
