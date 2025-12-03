package ui

import (
	"NovusTimeServer/lib"
	"fmt"
	"log"

	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/widget"
)

func test() {
	myApp := app.New()
	myWindow := myApp.NewWindow("Form Widget")

	entry := widget.NewEntry()
	textArea := widget.NewMultiLineEntry()
	textArea2 := widget.NewEntry()

	form := &widget.Form{
		Items: []*widget.FormItem{ // we can specify items in the constructor
			{Text: "Entry", Widget: entry}},
		OnSubmit: func() { // optional, handle form submission
			fmt.Println("Form submitted:", entry.Text)
			log.Println("multiline:", textArea.Text)

			textArea2.SetText(lib.GetIpv4Address(lib.GetManagedInterfaceName()))
			//myWindow.Close()
		},
	}

	// we can also append items
	form.Append("Text", textArea)

	form.Append("ip??", textArea2)

	myWindow.SetContent(form)
	myWindow.ShowAndRun()
}
