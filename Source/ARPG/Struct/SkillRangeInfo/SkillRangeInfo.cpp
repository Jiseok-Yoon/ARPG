#include "SkillRangeInfo.h"

FSkillRangeInfo::FSkillRangeInfo()
{
	bUseSphereTrace = true;
	TraceDirection = ESkillRangeDirection::Forward;
	TraceDistance = 200.0f;
	TraceRadius = 50.0f;
	CreateDelay = 0.15f;
}

FVector FSkillRangeInfo::GetTraceDirection(AActor* basedActor)
{
	switch (TraceDirection)
	{
	case ESkillRangeDirection::Forward:		return basedActor->GetActorForwardVector();
	case ESkillRangeDirection::Right:		return basedActor->GetActorRightVector();
	case ESkillRangeDirection::Backward:	return basedActor->GetActorForwardVector() * -1.0f;
	case ESkillRangeDirection::Left:		return basedActor->GetActorRightVector() * -1.0f;
	default:								return FVector::ZeroVector;
	}
}

TTuple<int32, float> FSkillRangeInfo::GetSkillCalcFormulaResult()
{
	/// Queue 를 사용한 대미지 계산
	// 현재 플레이어의 공격력 150

	if (DamageCalcFormula.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, 
			TEXT("FSkillRangeInfo.cpp :: %d LINE :: DamageCalcFormula is Empty! (DamageCalcFormula = %s)"),
			__LINE__, *DamageCalcFormula);

		return MakeTuple(1, 0.0f);
	}

	// 현재 플레이어 공격력을 나타냅니다. (테스트용)
	float atk = 150.0f;

	float result = 0;
	/// 아무것도 작성되지 않았을 경우 or H1 이 작성된 경우 타격 횟수를 1 로 설정합니다.
	/// H2 -> 2 회 타격
	/// H3 -> 3 회 타격
	/// H4 -> 4 회 타격
	/// H5 -> 5 회 타격
	/// ...
	/// H999 -> 999 회 타격
	/// 
	/// result = 200;
	/// result += (150 * 300%);
	/// result += (150 * 3)
	/// result -= 10;
	/// result *= 3;
	/// result = 3270


	// 연산 방식 상수
	const uint8 defaultType = 0b00000000;
	const uint8 addition = 0b00000001;
	const uint8 subtraction = 0b00000010;
	const uint8 multiplication = 0b00000100;
	const uint8 division = 0b00001000;

	// 연산 데이터들을 나타냅니다.
	/// - TTuple<연산 방식, 수치 데이터>
	TQueue<TTuple<uint8, FString>> calcSeq;

	// 연산 방식을 나타내기 위한 변수
	uint8 calcType = defaultType;

	// 수치를 나타내기 위한 변수
	FString calcValue;

	// 타격 횟수
	/// - 이 값이 0 이라면 타격 횟수를 읽는 상태로 인식시킵니다.
	int32 numberOfHits = 1;

	// 식을 읽어 연산 데이터를 생성합니다.
	for (int32 i = 0; i < DamageCalcFormula.Len() + 1; ++i)
	{
		// 모든 문자열을 읽었다면 마지막으로 데이터를 추가합니다.
		if (i == DamageCalcFormula.Len())
		{
			// 하나의 데이터를 큐에 추가합니다.
			calcSeq.Enqueue(MakeTuple(calcType, calcValue));
			break;
		}

		// 문자 하나를 읽습니다.
		TCHAR read = DamageCalcFormula[i];

		// 공백을 읽었다면 하나의 데이터의 끝으로 인식합니다.
		if (read == TEXT(' '))
		{
			// 타격 횟수를 모두 읽었다면
			if (numberOfHits == 0)
			{
				// 타격 횟수를 저장합니다.
				numberOfHits = FCString::Atoi(*calcValue);
				if (numberOfHits <= 0) numberOfHits = 1;
				/// - 타격 횟수가 0 이하의 값으로 설정될 수 없도록 합니다.
				calcValue = nullptr;
			}

			else if (!calcValue.IsEmpty())
			{
				// 하나의 데이터를 큐에 추가합니다.
				calcSeq.Enqueue(MakeTuple(calcType, calcValue));

				// 연산 방식, 데이터를 비웁니다.
				calcType = defaultType;
				calcValue = nullptr;
			}
		}
		// 타격 횟수
		else if (read == TEXT('h') || read == TEXT('H'))
			numberOfHits = 0;

		// 산술 연산자
		else if (read == TEXT('+'))
			calcType = addition;
		else if (read == TEXT('-'))
			calcType = subtraction;
		else if (read == TEXT('*'))
			calcType = multiplication;
		else if (read == TEXT('/'))
			calcType = division;

		// 나머지 데이터 추가
		else calcValue += read;
	}




	// 연산합니다.
	TTuple<uint8, FString> calcData;
	while (!calcSeq.IsEmpty())
	{
		calcSeq.Dequeue(calcData);

		// 연산 방식을 얻습니다.
		calcType = calcData.Get<0>();

		// 수치 문자열을 얻습니다.
		calcValue = calcData.Get<1>();

		// str 을 실제 계산에 사용되는 수치로 변환합니다.
		Func(float, fnToValue, (FString)) = [](FString str)
		{
			// 수치 데이터가 아니라면 첫 번째 문자를 제거합니다.
			/// - ex) M3 -> 3
			/// - ex) P300 -> 300
			if (!str.IsNumeric()) str.RemoveAt(0);
			return FCString::Atof(*str);
		};

		// 실제 계산에 사용되는 수치 데이터를 나타냅니다.
		float value = 0.0f;

		// 퍼댐
		if (calcValue.Contains(TEXT("p")))
			value = atk * 0.01f * fnToValue(calcValue);

		// 기본 공격력 X N
		else if (calcValue.Contains(TEXT("d")))
			value = atk * fnToValue(calcValue);

		// 수치 계산
		else value = FCString::Atof(*calcValue);

		switch (calcType)
		{
		case defaultType:
			result = value;
			break;
		case addition:
			result += value;
			break;
		case subtraction:
			result -= value;
			break;
		case multiplication:
			result *= value;
			break;
		case division:
			result /= value;
			break;
		}
	}

	return MakeTuple(numberOfHits, result);
}
