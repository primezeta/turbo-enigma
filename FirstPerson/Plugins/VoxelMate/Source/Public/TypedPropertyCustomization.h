#pragma once
#include "EngineMinimal.h"
#include "TypedProperty.h"
#include "SlateBasics.h"
#include "PropertyEditing.h"

#define LOCTEXT_NAMESPACE "TypedPropertyCustomization"

class FTypedPropertyCustomization : public IPropertyTypeCustomization
{
public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance()
    {
        return MakeShareable(new FTypedPropertyCustomization());
    }

    /** IPropertyTypeCustomization interface */
    virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
    {
        uint32 NumChildren;
        StructPropertyHandle->GetNumChildren(NumChildren);

        for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
        {
            const TSharedRef<IPropertyHandle> ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();

            if (ChildHandle->GetProperty()->GetName() == TEXT("SomeUProperty"))
            {
                PropertyHandle = ChildHandle;
            }
        }
        check(PropertyHandle.IsValid());

        FDetailWidgetDecl& NameContent = HeaderRow.NameContent();
        TSharedRef<SWidget> NameWidget = StructPropertyHandle->CreatePropertyNameWidget(LOCTEXT("Property header", "***TODO*** Some name"));
        FDetailWidgetDecl& ValueContent = NameContent[NameWidget].ValueContent();

        FDetailWidgetDecl& Pane = ValueContent.MinDesiredWidth(500.0f);
        TSharedRef<STextBlock> TextBlock = SNew(STextBlock);
        TextBlock->SetText(LOCTEXT("Extra info", "Some new representation"));
        TextBlock->SetFont(IDetailLayoutBuilder::GetDetailFont());
        Pane[TextBlock];
    }

    virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
    {

    }

private:
    TSharedPtr<IPropertyHandle> PropertyHandle;
};

#undef LOCTEXT_NAMESPACE
