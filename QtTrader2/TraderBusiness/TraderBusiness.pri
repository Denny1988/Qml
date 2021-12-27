# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.
message("You are running qmake on TraderBusiness.pri")


HEADERS += $$PWD/Business/BusinessHelper.h \
    $$PWD/Business/ComputingManager.h \
    $$PWD/Business/InitializeManager.h \
    $$PWD/Business/Model/Chat.h \
    $$PWD/Business/Model/Computing/ChangeFund.h \
    $$PWD/Business/Model/Computing/ComputingAccount.h \
    $$PWD/Business/Model/Computing/ComputingInstrument.h \
    $$PWD/Business/Model/Computing/ComputingOrder.h \
    $$PWD/Business/Model/Computing/ComputingResult.h \
    $$PWD/Business/Model/Computing/DemandComputing.h \
    $$PWD/Business/Model/Computing/OverridedQuotation.h \
    $$PWD/Business/Model/IInstantMessage.h \
    $$PWD/Business/Model/InitialQuotation.h \
    $$PWD/Business/Model/Log.h \
    $$PWD/Business/Model/News.h \
    $$PWD/Business/Model/PriceAlert.h \
    $$PWD/Business/Model/Server/Account.h \
    $$PWD/Business/Model/Server/BOBetType.h \
    $$PWD/Business/Model/Server/BOPolicy.h \
    $$PWD/Business/Model/Server/BOPolicyDetail.h \
    $$PWD/Business/Model/Server/CounterParty.h \
    $$PWD/Business/Model/Server/Customer.h \
    $$PWD/Business/Model/Server/DealingPolicyDetail.h \
    $$PWD/Business/Model/Server/DeliveryCharge.h \
    $$PWD/Business/Model/Server/DeliverySpecificationDetail.h \
    $$PWD/Business/Model/Server/InstalmentPolicy.h \
    $$PWD/Business/Model/Server/InstalmentPolicyDetail.h \
    $$PWD/Business/Model/Server/Instrument.h \
    $$PWD/Business/Model/Server/InstrumentUnit.h \
    $$PWD/Business/Model/Server/Organization.h \
    $$PWD/Business/Model/Server/PhysicalPaymentDiscount.h \
    $$PWD/Business/Model/Server/PhysicalPaymentDiscountDetail.h \
    $$PWD/Business/Model/Server/QuotePolicy.h \
    $$PWD/Business/Model/Server/QuotePolicyDetail.h \
    $$PWD/Business/Model/Server/SystemParameter.h \
    $$PWD/Business/Model/Server/TradePolicyDetail.h \
    $$PWD/Business/Model/Server/VolumeNecessary.h \
    $$PWD/Business/Model/Server/VolumeNecessaryDetail.h \
    $$PWD/Business/Model/Setting/BOPolicySetting.h \
    $$PWD/Business/Model/Setting/BusinessAccount.h \
    $$PWD/Business/Model/Setting/BusinessInstrument.h \
    $$PWD/Business/Model/Setting/CalculateQuotationDefaultSetting.h \
    $$PWD/Business/Model/Setting/CalculateSetting.h \
    $$PWD/Business/Model/Setting/Currency.h \
    $$PWD/Business/Model/Setting/CurrencyRate.h \
    $$PWD/Business/Model/Setting/DeliveryCharge.h \
    $$PWD/Business/Model/Setting/DeliveryHoliday.h \
    $$PWD/Business/Model/Setting/DeliverySpecificationDetail.h \
    $$PWD/Business/Model/Setting/InstalmentSetting.h \
    $$PWD/Business/Model/Setting/InstalmentSettingDetail.h \
    $$PWD/Business/Model/Setting/InstrumentColumnSetting.h \
    $$PWD/Business/Model/Setting/InstrumentParameter.h \
    $$PWD/Business/Model/Setting/InterestRate.h \
    $$PWD/Business/Model/Setting/LeverageSetting.h \
    $$PWD/Business/Model/Setting/LotSetting.h \
    $$PWD/Business/Model/Setting/MarginSetting.h \
    $$PWD/Business/Model/Setting/MovePipsInfoSetting.h \
    $$PWD/Business/Model/Setting/OpenOrderListColumnSetting.h \
    $$PWD/Business/Model/Setting/PaymentInstructionIntegralitySetting.h \
    $$PWD/Business/Model/Setting/PhysicalPaymentDiscountSetting.h \
    $$PWD/Business/Model/Setting/PhysicalPaymentDiscountSettingDetail.h \
    $$PWD/Business/Model/Setting/PlaceSetting.h \
    $$PWD/Business/Model/Setting/PlaceSettingDetail.h \
    $$PWD/Business/Model/Setting/ScrapInstrument.h \
    $$PWD/Business/Model/Setting/SettingDataBase.h \
    $$PWD/Business/Model/Setting/SystemSetting.h \
    $$PWD/Business/Model/Setting/TradeDay.h \
    $$PWD/Business/Model/Setting/TradingListColumnsVisibility.h \
    $$PWD/Business/Model/Setting/VolumeNecessary.h \
    $$PWD/Business/Model/Setting/VolumeNecessaryDetail.h \
    $$PWD/Business/Model/Trading/DeliveryRequest.h \
    $$PWD/Business/Model/Trading/DeliveryRequestOrderRelation.h \
    $$PWD/Business/Model/Trading/Fund.h \
    $$PWD/Business/Model/Trading/InstrumentAccountSummary.h \
    $$PWD/Business/Model/Trading/InstrumentSummary.h \
    $$PWD/Business/Model/Trading/InterestFreeDay.h \
    $$PWD/Business/Model/Trading/InventoryData.h \
    $$PWD/Business/Model/Trading/MarketingGroupFund.h \
    $$PWD/Business/Model/Trading/OrderInstalmentData.h \
    $$PWD/Business/Model/Trading/OrderRelation.h \
    $$PWD/Business/Model/Trading/PendingInventoryData.h \
    $$PWD/Business/Model/Trading/ScrapDeposit.h \
    $$PWD/Business/Model/Trading/ShortSellData.h \
    $$PWD/Business/Model/Trading/SubFund.h \
    $$PWD/Business/Model/Trading/TradingOrder.h \
    $$PWD/Business/Model/Trading/Transaction.h \
    $$PWD/Business/QuotationManager.h \
    $$PWD/Business/ServerSourceManager.h \
    $$PWD/Business/SettingManager.h \
    $$PWD/Business/TradingManager.h \
    $$PWD/Common/AppSetting.h \
    $$PWD/Common/LogHelper.h \
    $$PWD/Common/BaseWidgetStyle.h \
    $$PWD/Common/Constants/ButtonPixmapConstants.h \
    $$PWD/Common/Constants/ColorBrushConstants.h \
    $$PWD/Common/Constants/CommunicationConstants.h \
    $$PWD/Common/Constants/DateTimeConstants.h \
    $$PWD/Common/Constants/FunctionConstants.h \
    $$PWD/Common/Constants/LogKeyConstants.h \
    $$PWD/Common/Constants/MarginConstants.h \
    $$PWD/Common/Constants/MethodNameConstants.h \
    $$PWD/Common/Constants/NumberConstants.h \
    $$PWD/Common/Constants/QuotationConstants.h \
    $$PWD/Common/Constants/ViewColumnsConstants.h \
    $$PWD/Common/Constants/ViewObjectNameConstants.h \
    $$PWD/Common/CustomerSetting.h \
    $$PWD/Common/EncryptHelper.h \
    $$PWD/Common/Encrypt/AesCipher.h \
    $$PWD/Common/Encrypt/Base64.h \
    $$PWD/Common/Encrypt/md5.h \
    $$PWD/Common/Enum/AccountEnum.h \
    $$PWD/Common/Enum/AccountOrderTypeEnum.h \
    $$PWD/Common/Enum/BOEnum.h \
    $$PWD/Common/Enum/CancelReasonEnum.h \
    $$PWD/Common/Enum/CommonEnum.h \
    $$PWD/Common/Enum/CommunicationEnum.h \
    $$PWD/Common/Enum/FrameworkEnum.h \
    $$PWD/Common/Enum/InformationCenterEnum.h \
    $$PWD/Common/Enum/InstalmentEnum.h \
    $$PWD/Common/Enum/InstrumentEnum.h \
    $$PWD/Common/Enum/MarginEnum.h \
    $$PWD/Common/Enum/MarketInfoTypeEnum.h \
    $$PWD/Common/Enum/PhysicalEnum.h \
    $$PWD/Common/Enum/PlaceEnum.h \
    $$PWD/Common/Enum/PriceAlertEnum.h \
    $$PWD/Common/Enum/LogLevelEnum.h \
    $$PWD/Common/Enum/QuotationMenuTypeEnum.h \
    $$PWD/Common/Enum/SettingOptionEnum.h \
    $$PWD/Common/Enum/StatusEnum.h \
    $$PWD/Common/Enum/TimeOptionInTraderLogEnum.h \
    $$PWD/Common/Enum/TradingListNameEnum.h \
    $$PWD/Common/Enum/TradingOrderEnum.h \
    $$PWD/Common/Enum/TransactionEnum.h \
    $$PWD/Common/Enum/TransactionErrorEnum.h \
    $$PWD/Common/Enum/UIEnum.h \
    $$PWD/Common/GuidMappingManager.h \
    $$PWD/Common/LoginSetting.h \
    $$PWD/Common/Parameter.h \
    $$PWD/Common/Price.h \
    $$PWD/Common/Result/Result.h \
    $$PWD/Common/Struct/CommonStruct.h \
    $$PWD/Common/Struct/InformationStruct.h \
    $$PWD/Common/Struct/InstrumentStateStruct.h \
    $$PWD/Common/Struct/LoginSettingStruct.h \
    $$PWD/Common/Struct/PhysicalStruct.h \
    $$PWD/Common/Struct/PlacingStruct.h \
    $$PWD/Common/Struct/SettingStruct.h \
    $$PWD/Common/TraderBaseInfo.h \
    $$PWD/Common/Util/Convert_Common.h \
    $$PWD/Common/Util/PriceHelper.h \
    $$PWD/Communication/Package.h \
    $$PWD/traderbusiness_global.h \
    $$PWD/Communication/Heartbeater.h \
    $$PWD/Business/BusinessManager.h \
    $$PWD/Common/Result/ChartCountResult.h \
    $$PWD/Common/Result/ChartXmlDataResult.h \
    $$PWD/Common/Result/InvokeResult.h \
    $$PWD/Common/Result/PlaceResult.h \
    $$PWD/Common/Result/UpdatePriceAlertResult.h \
    $$PWD/Common/SignalForwarder.h \
    $$PWD/Common/StatusManager.h \
    $$PWD/Common/ThreadManager.h \
    $$PWD/Common/TimerManager.h \
    $$PWD/Common/UtcTimeManager.h \
    $$PWD/Communication/SocketWrapper.h \
    $$PWD/Communication/SSLChannel.h \
    $$PWD/Service/ChartService.h \
    $$PWD/Service/ProvisionalServiceManager.h \
    $$PWD/Service/QuotationService.h \
    $$PWD/Service/ReportService.h \
    $$PWD/Service/ServiceManager.h \
    $$PWD/Service/TradeService.h \
    $$PWD/Common/LogHelper.h \
    $$PWD/Business/Model/Server/AdministrativeCharge.h \
    $$PWD/Business/Model/Server/AdministrativeChargeDetail.h \
    $$PWD/Business/Model/Server/PricingPolicyDetail.h \
    $$PWD/Business/Model/Setting/PricingPolicyDetail.h \
    $$PWD/Common/Struct/MarginStruct.h

SOURCES += $$PWD/Business/BusinessHelper.cpp \
    $$PWD/Business/BusinessManager.cpp \
    $$PWD/Business/ComputingManager.cpp \
    $$PWD/Business/InitializeManager.cpp \
    $$PWD/Business/Model/Computing/ComputingAccount.cpp \
    $$PWD/Business/Model/Computing/ComputingInstrument.cpp \
    $$PWD/Business/Model/Computing/ComputingOrder.cpp \
    $$PWD/Business/Model/Computing/OverridedQuotation.cpp \
    $$PWD/Business/Model/Setting/BusinessAccount.cpp \
    $$PWD/Business/Model/Setting/BusinessInstrument.cpp \
    $$PWD/Business/Model/Setting/InstalmentSetting.cpp \
    $$PWD/Business/Model/Setting/PhysicalPaymentDiscountSetting.cpp \
    $$PWD/Business/Model/Setting/PlaceSetting.cpp \
    $$PWD/Business/Model/Setting/SystemSetting.cpp \
    $$PWD/Business/Model/Setting/VolumeNecessary.cpp \
    $$PWD/Business/Model/Trading/InstrumentSummary.cpp \
    $$PWD/Business/Model/Trading/InventoryData.cpp \
    $$PWD/Business/Model/Trading/PendingInventoryData.cpp \
    $$PWD/Business/Model/Trading/ShortSellData.cpp \
    $$PWD/Business/Model/Trading/TradingOrder.cpp \
    $$PWD/Business/Model/Trading/Transaction.cpp \
    $$PWD/Business/QuotationManager.cpp \
    $$PWD/Business/ServerSourceManager.cpp \
    $$PWD/Business/SettingManager.cpp \
    $$PWD/Business/TradingManager.cpp \
    $$PWD/Common/AppSetting.cpp \
    $$PWD/Common/Constants/ButtonPixmapConstants.cpp \
    $$PWD/Common/Constants/ColorBrushConstants.cpp \
    $$PWD/Common/Constants/CommunicationConstants.cpp \
    $$PWD/Common/Constants/DateTimeConstants.cpp \
    $$PWD/Common/Constants/FunctionConstants.cpp \
    $$PWD/Common/Constants/LogKeyConstants.cpp \
    $$PWD/Common/Constants/MarginConstants.cpp \
    $$PWD/Common/Constants/MethodNameConstants.cpp \
    $$PWD/Common/Constants/NumberConstants.cpp \
    $$PWD/Common/Constants/QuotationConstants.cpp \
    $$PWD/Common/Constants/ViewColumnsConstants.cpp \
    $$PWD/Common/Constants/ViewObjectNameConstants.cpp \
    $$PWD/Common/CustomerSetting.cpp \
    $$PWD/Common/Encrypt/AesCipher.cpp \
    $$PWD/Common/Encrypt/Base64.cpp \
    $$PWD/Common/Encrypt/md5.cpp \
    $$PWD/Common/GuidMappingManager.cpp \
    $$PWD/Common/LoginSetting.cpp \
    $$PWD/Common/Result/ChartCountResult.cpp \
    $$PWD/Common/Result/ChartXmlDataResult.cpp \
    $$PWD/Common/Result/InvokeResult.cpp \
    $$PWD/Common/Result/PlaceResult.cpp \
    $$PWD/Common/Result/UpdatePriceAlertResult.cpp \
    $$PWD/Common/SignalForwarder.cpp \
    $$PWD/Common/StatusManager.cpp \
    $$PWD/Common/ThreadManager.cpp \
    $$PWD/Common/TimerManager.cpp \
    $$PWD/Common/TraderBaseInfo.cpp \
    $$PWD/Common/UtcTimeManager.cpp \
    $$PWD/Communication/Heartbeater.cpp \
    $$PWD/Communication/SocketWrapper.cpp \
    $$PWD/Communication/SSLChannel.cpp \
    $$PWD/Communication/Package.cpp \
    $$PWD/Service/ChartService.cpp \
    $$PWD/Service/ProvisionalServiceManager.cpp \
    $$PWD/Service/QuotationService.cpp \
    $$PWD/Service/ReportService.cpp \
    $$PWD/Service/ServiceManager.cpp \
    $$PWD/Service/TradeService.cpp \
    $$PWD/Common/Util/PriceHelper.cpp