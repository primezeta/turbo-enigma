#pragma once
#include "EngineMinimal.h"
#include "TypedProperty.h"
#include "VoxelDatabaseProxy.h"
#include "SlateBasics.h"
#include "STextComboBox.h"
#include "PropertyEditing.h"

#define LOCTEXT_NAMESPACE "TypedPropertyCustomization"

class FTypedPropertyCustomization : public IPropertyTypeCustomization
{
public:
    FTypedPropertyCustomization()
    {
        FillEnumOptions(TypeSelectionOptions, GetEnum(TEXT("EVoxelDatabaseType")));
    }

    static TSharedRef<IPropertyTypeCustomization> MakeInstance()
    {
        return MakeShareable(new FTypedPropertyCustomization());
    }

    static UEnum& GetEnum(const FName& Name)
    {
        //const FName& Name(TEXT("EMeshFeatureImportance::Off"));
        static UEnum* Enum = NULL;
        if (Enum == NULL)
        {
            UEnum::LookupEnumName(Name, &Enum);
            check(Enum);
        }
        return *Enum;
    }

    static void FillEnumOptions(TArray<TSharedPtr<FString>>& OutStrings, UEnum& InEnum)
    {
        const int32& NumEnums = InEnum.NumEnums();
        for (int32 EnumIndex = 0; EnumIndex < NumEnums - 1; ++EnumIndex)
        {
            OutStrings.Add(MakeShareable(new FString(InEnum.GetEnumName(EnumIndex))));
        }
    }

    void OnTypeChanged(TSharedPtr<FString> Selection, ESelectInfo::Type SelectType)
    {
        //if (TypedPropertyHandle.IsValid())
        //{
        //    UProperty* Prop = TypedPropertyHandle->GetProperty();
        //    const int32 Index = TypedPropertyHandle->GetIndexInArray();
        //    UProperty* TypedProperty = nullptr;
        //    UStruct* Struct = nullptr;
        //    UVoxelDatabaseProxy* VoxelDatabaseProxy = nullptr;
        //    if (Prop != nullptr &&
        //        TypedProperty = Prop->GetOwnerProperty() &&
        //        Struct = TypedProperty->GetOwnerStruct() &&
        //        VoxelDatabaseProxy = TypedProperty->GetTypedOuter<UVoxelDatabaseProxy>())
        //    {
        //        const EVoxelDatabaseType& Type = VoxelDatabaseProxy->Grids[Index].Type;
        //        if (Type != EVoxelDatabaseType::NoneType)
        //        {
        //            for (TFieldIterator<UProperty> i(Struct); i; ++i)
        //            {
        //                TArray<FString> Categories;
        //                GetPropertyCategories(*i, Categories);
        //                if (Categories.Find(TEXT("VoxelDatabaseValue")))
        //                {
        //                    const UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, UTF8_TO_TCHAR(typeid(EVoxelDatabaseType).name()));
        //                    check(Enum != nullptr);
        //                    for (uint8 j = 0; j < Enum->GetMaxEnumValue(); ++j)
        //                    {
        //                        const FString EnumName = Enum->GetEnumNameStringByValue((int32)j);
        //                        if (Categories.Find(EnumName))
        //                        {
        //                            const FString ChildName = EnumName + TEXT("Value");
        //                            TSharedPtr<IPropertyHandle> ChildProp = TypedPropertyHandle->GetChildHandle(FName(*ChildName), false);
        //                            ChildProp->
        //                        }
        //                    }
        //                }
        //            }
        //        }
        //    }
        //}
    }

    FReply OnClickedEditButton()
    {
        return FReply::Handled();
    }

    /** IPropertyTypeCustomization interface */
    virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
    {
        TypedPropertyHandle = PropertyHandle;
        HeaderRow
            .NameContent()
            [
                SNew(STextBlock)
                .Font(IDetailLayoutBuilder::GetDetailFont())
                .Text(LOCTEXT("TypedPropertyCustomization", "Type"))
            ]
        .ValueContent()
        [
            SAssignNew(Overlay, SOverlay)
            + SOverlay::Slot()
                .HAlign(EHorizontalAlignment::HAlign_Left)
                .VAlign(EVerticalAlignment::VAlign_Top)
                [
                    SAssignNew(TypeSelectionCombo, STextComboBox)
                        .ContentPadding(0)
                        .OptionsSource(&TypeSelectionOptions)
                        .InitiallySelectedItem(TypeSelectionOptions[0])
                        .OnSelectionChanged(this, &FTypedPropertyCustomization::OnTypeChanged)
                ]
            + SOverlay::Slot()
                .HAlign(EHorizontalAlignment::HAlign_Right)
                .VAlign(EVerticalAlignment::VAlign_Bottom)
                [
                    SAssignNew(EditButton, SButton)
                        .Text(FText::FromString(TEXT("...")))
                        .OnClicked(this, &FTypedPropertyCustomization::OnClickedEditButton)
                ]
        ];
    }

    virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
    {

    }

    void GetPropertyCategories(UProperty* Property, TArray<FString>& OutCategories)
    {
        static const TCHAR* Delim = TEXT("|");
        if (Property)
        {
            const FString& Categories = Property->GetMetaData(FName(TEXT("Category")));
            Categories.ParseIntoArray(OutCategories, Delim);
        }
    }

private:
    TSharedPtr<SOverlay> Overlay;
    TArray<TSharedPtr<FString>> TypeSelectionOptions;
    TSharedPtr<STextComboBox> TypeSelectionCombo;
    TSharedPtr<SButton> EditButton;
    TSharedPtr<IPropertyHandle> TypedPropertyHandle;
};

#undef LOCTEXT_NAMESPACE
