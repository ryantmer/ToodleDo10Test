import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2

Page {
    titleBar: TitleBar {
        title: "Sync for Toodledo - Contexts"
    }
    
    Container {
        ListView {
            id: contextListView
            layout: StackListLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            
            dataModel: app.contextDataModel
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    Container {
                        id: itemContainer
                        StandardListItem {
                            title: ListItemData.name
                            description: contextItemContainer.ListItem.view.privateString(ListItemData.private)
                            
                            contextActions: [
                                ActionSet {
                                    DeleteActionItem {
                                        onTriggered: {
                                            deleteConfirmDialog.show();
                                        }
                                    }
                                }
                            ]
                            
                            attachedObjects: [
                                SystemDialog {
                                    id: deleteConfirmDialog
                                    title: "Delete Folder"
                                    body: "Are you sure you want to delete this context?"
                                    confirmButton.label: "Delete"
                                    confirmButton.enabled: true
                                    cancelButton.label: "Cancel"
                                    cancelButton.enabled: true
                                    onFinished: {
                                        if (result == SystemUiResult.ConfirmButtonSelection) {
                                            var contextData = {"id": ListItemData.id}
                                            app.contextDataModel.remove(contextData);
                                        }
                                    }
                                }
                            ]
                        }
                    }
                }
            ]
            
            onTriggered: {
                var context = dataModel.data(indexPath);
                var page = addEditFolderDefinition.createObject();
                page.data = context;
                page.edit = true;
                page.setup();
                mainNavPane.push(page);
            }
            
            function privateString(isPrivate) {
                if (isPrivate) {
                    return "Private";
                } else {
                    return "Not Private";
                }
            }
        }
    }
    
    actions: [
        ActionItem {
            title: "Refresh"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_reload.png"
            onTriggered: {
                app.contextDataModel.refresh();
            }
        },
        ActionItem {
            title: "Add Folder"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_add.png"
            onTriggered: {
                var page = addEditFolderDefinition.createObject();
                page.edit = false;
                page.setup();
                mainNavPane.push(page);
            }
        }
    ]
    
    attachedObjects: [
        ComponentDefinition {
            id: addEditFolderDefinition
            content: AddEditFolder{}
        }
    ]
}