import bb.cascades 1.2

Sheet {
    id: editTaskSheet
    
    property alias taskId: taskId.text
    property alias completed: taskCompleted.checked
    property alias title: taskName.text
    property alias duedate: taskDueDate.value
    property alias notes: taskNotes.text
    
    Page {
        titleBar: TitleBar {
            title: "Edit Task"
            visibility: ChromeVisibility.Visible
            dismissAction: ActionItem {
                title: "Cancel"
                onTriggered: {
                    editTaskSheet.close();
                }
            }
                
            acceptAction: ActionItem {
                title: "Save"
                property variant taskData
                
                onTriggered: {
                    taskData = {"id": taskId.text,
                                "completed": taskCompleted.checked,
                                "title": taskName.text,
                                "duedate": app.dateTimeToUnixTime(taskDueDate.value),
                                "notes": taskNotes.text}
                    app.editTask(taskData);
                    editTaskSheet.close();
                }
            }
        }
            
        Container {
            layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
            topPadding: 10.0
            leftPadding: 10.0
            rightPadding: 10.0
            bottomPadding: 10.0
                
            CheckBox {
                id: taskCompleted
                text: "Completed"
            }
            TextField {
                //TODO: Remove this, get taskId from server
                id: taskId
                hintText: "Task ID number"
            }
            TextField {
                id: taskName
                hintText: "Task Name"
                horizontalAlignment: HorizontalAlignment.Fill
                topMargin: 10.0
                leftMargin: 10.0
                rightMargin: 10.0
                bottomMargin: 10.0
            }
            DateTimePicker {
                id: taskDueDate
                horizontalAlignment: HorizontalAlignment.Fill
                mode: DateTimePickerMode.Date
                expanded: true
                title: "Due Date"
                topMargin: 10.0
                leftMargin: 10.0
                rightMargin: 10.0
                bottomMargin: 10.0
            }
            TextArea {
                id: taskNotes
                hintText: "Detailed notes about task"
                horizontalAlignment: HorizontalAlignment.Fill
                topMargin: 10.0
                leftMargin: 10.0
                rightMargin: 10.0
                bottomMargin: 10.0
            }
        }
    }
}