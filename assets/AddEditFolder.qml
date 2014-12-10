import bb.cascades 1.2

Page {
    id: addEditFolderPage
    
    property variant data;
    property bool edit;
    
    function setup() {
        if (edit) {
            folderName.text = data.name;
            folderPrivate.checked = data.private;
            folderArchived.checked = data.archived;
            
            addSaveButton.title = "Save";
            addSaveButton.imageSource = "asset:///images/ic_save.png";
        }
    }
    
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: "Cancel"
            onTriggered: {
                mainNavPane.pop();
            }
        }
    }
    
    actions: [
        ActionItem {
            id: addSaveButton
            title: "Add" //Changed to "Save" in setup if editing a folder
            imageSource: "asset:///images/ic_add.png" //Changed in setup if editing a folder
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (!folderName.text) {
                    titleRequired.visible = true;
                    return;
                }
                if (folderName.text.length > 32) {
                    nameTooLong.visible = true;
                    return;
                }
                
                if (edit) {
                    var folderData = {"id": data.id,
                                "name": folderName.text,
                                "private": folderPrivate.checked + 0,
                                "archived": folderArchived.checked + 0,
                                "ord": data.ord};
                    app.folderDataModel.edit(data, folderData);
                } else {
                    var folderData = {"name": folderName.text,
                                "private": folderPrivate.checked + 0};
                    app.folderDataModel.add(folderData);
                }
                mainNavPane.pop();
            }
        }
    ]
    
    ScrollView {
        scrollViewProperties {
            scrollMode: ScrollMode.Vertical
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            topPadding: 20
            leftPadding: 20
            rightPadding: 20
            bottomPadding: 20
            
            Label {
                id: titleRequired
                text: "Required"
                textStyle.color: Color.Red
                visible: false
            }
            Label {
                id: nameTooLong
                text: "Max 32 characters"
                textStyle.color: Color.Red
                visible: false
            }
            TextField {
                id: folderName
                hintText: "Folder Name"
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 30
            }
            CheckBox {
                id: folderArchived
                text: "Archived"
                bottomMargin: 30
                visible: edit //only show when editing a folder
            }
            CheckBox {
                id: folderPrivate
                text: "Private"
                bottomMargin: 30
            }
        }
    }
}
