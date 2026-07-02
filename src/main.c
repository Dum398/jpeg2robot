#include <gtk/gtk.h>
#include <stdio.h>
GdkPixbuf *imidz_pojntr;
GtkWidget *window;
GtkWidget *box;
GtkWidget *imidztukonvert;




void displayimage() {
    if (imidz_pojntr) {
        if (imidztukonvert) {
            gtk_widget_destroy(imidztukonvert);
            imidztukonvert = NULL;
        }
        imidztukonvert = gtk_image_new_from_pixbuf(imidz_pojntr);
        gtk_box_pack_start(GTK_BOX(box), imidztukonvert, TRUE, TRUE, 50);
        gtk_widget_show_all(window);

    }
}



void selectfile(GtkButton *button3, gpointer user_data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open Image", GTK_WINDOW(user_data), GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        GError *err = NULL;
        imidz_pojntr = gdk_pixbuf_new_from_file(filename, &err);
            if (imidz_pojntr) {
                g_print("Nacteno");
                displayimage();
                
            } else {
                g_print("Error");
                g_error_free(err);
            }
    }

    gtk_widget_destroy(dialog);

}

void convert(GtkButton *button1, gpointer user_data) {
    if (imidz_pojntr) {
        if (imidztukonvert) {
            gtk_widget_destroy(imidztukonvert);
            imidztukonvert = NULL;
        }
        GdkPixbuf *saturated = gdk_pixbuf_copy(imidz_pojntr);
        gdk_pixbuf_saturate_and_pixelate(imidz_pojntr, saturated, 0.0, FALSE);
        imidztukonvert = gtk_image_new_from_pixbuf(saturated);
        gtk_box_pack_start(GTK_BOX(box), imidztukonvert, TRUE, TRUE, 50);
        gtk_widget_show_all(window);
        g_object_unref(saturated);
    }
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);


    
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
    


    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    button1 = gtk_button_new_with_label("Convert!");
    button2 = gtk_button_new_with_label("Exit");
    button3 = gtk_button_new_with_label("Open File");
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(button1, "clicked", G_CALLBACK(convert), NULL);
    g_signal_connect(button2, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(button3, "clicked", G_CALLBACK(selectfile), NULL);


    gtk_window_set_title(GTK_WINDOW(window), "Konverter obrazku do souradnic pro robota");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_box_pack_start(GTK_BOX(box), button1, TRUE, TRUE, 50);
    gtk_box_pack_start(GTK_BOX(box), button2, TRUE, TRUE, 50);
    gtk_box_pack_start(GTK_BOX(box), button3, TRUE, TRUE, 50);

    gtk_widget_show_all(window);

    gtk_main();


};
