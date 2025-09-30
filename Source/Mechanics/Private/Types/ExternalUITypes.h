// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Types/MessageTypes.h"
#include "Online/CoreOnline.h"
#include "UObject/Interface.h"

#include "ExternalUITypes.generated.h"

USTRUCT()
struct MECHANICS_API FExternalUIFlowHandlerRegistration
{
    GENERATED_BODY()

public:
    UPROPERTY()
    UObject *Interface;
    FDelegateHandle LoginFlowUIRequired;
    FDelegateHandle CreateAccountFlowUIRequired;
};

USTRUCT(BlueprintType)
struct MECHANICS_API FLoginFlowResultBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString Token;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FText ErrorMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString ErrorRaw;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 NumericErrorCode;

    static FLoginFlowResultBP FromNative(const FLoginFlowResult &InObj);
    FLoginFlowResult ToNative();
};

UINTERFACE(BlueprintType)
class MECHANICS_API UExternalUIFlowHandler : public UInterface
{
    GENERATED_BODY()
};

class MECHANICS_API IExternalUIFlowHandler
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, Category = "Online|External UI")
    bool OnLoginFlowUIRequired(
        const FString &RequestedURL,
        class UOnlineExternalUISubsystem *ExternalUIContext,
        int RequestID);

    UFUNCTION(BlueprintNativeEvent, Category = "Online|External UI")
    bool OnCreateAccountFlowUIRequired(
        const FString &RequestedURL,
        class UOnlineExternalUISubsystem *ExternalUIContext,
        int RequestID);
};

USTRUCT(BlueprintType) struct MECHANICS_API FShowWebUrlParameters
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool Embedded;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool ShowCloseButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool ShowBackground;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool HideCursor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool ResetCookies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 OffsetX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 OffsetY;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 SizeX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 SizeY;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    TArray<FString> AllowedDomains;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString CallbackPath;

    static FShowWebUrlParameters FromNative(const FShowWebUrlParams &InObj);
    FShowWebUrlParams ToNative();
};

USTRUCT(BlueprintType)
struct MECHANICS_API FShowStoreParameters
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString ProductId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool AddToCart;

    static FShowStoreParameters FromNative(const FShowStoreParams &InObj);
    FShowStoreParams ToNative();
};

USTRUCT(BlueprintType)
struct MECHANICS_API FShowSendMessageParameters
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FText DisplayTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    TMap<FString, FString> DisplayTitle_Loc;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FText DisplayMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FText DisplayDetails;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    TMap<FString, FString> DisplayDetails_Loc;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FOnlineMessagePayloadData DataPayload;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 MaxRecipients;

    static FShowSendMessageParameters FromNative(const FShowSendMessageParams &InObj);
    FShowSendMessageParams ToNative();
};

UENUM(BlueprintType, Meta = (ScriptName = "EPlatformMessageType"))
enum class EPlatformMessageType_ : uint8
{
    EmptyStore,
    ChatRestricted,
    UGCRestricted
};