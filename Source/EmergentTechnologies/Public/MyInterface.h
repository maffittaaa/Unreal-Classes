#pragma once

#include "CoreMinimal.h"
#include "MyInterface.generated.h"

UINTERFACE(BlueprintType)

class UMyInterface: public UInterface {
	GENERATED_BODY()
};

class EMERGENTTECHNOLOGIES_API IMyInterface {
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
		void TakeDamageFromObject(float damageAmount, AActor* damageCauser);
};