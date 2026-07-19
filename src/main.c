#include <gtk/gtk.h>
#include <stdio.h>


GdkPixbuf *imidz_pojntr;
GtkWidget *window;
GtkWidget *imidztukonvert;
GtkWidget *slider;
GError *err = NULL;
GtkWidget *box1;
int compression;

FILE *scriptfile;


void displayimage() {
    if (imidz_pojntr) {
        if (imidztukonvert) {
            gtk_widget_destroy(imidztukonvert);
            imidztukonvert = NULL;
        }
        imidztukonvert = gtk_image_new_from_pixbuf(imidz_pojntr);
        gtk_box_pack_start(GTK_BOX(box1), imidztukonvert, TRUE, TRUE, 50);
        gtk_widget_show_all(window);

    }
}



void selectfile(GtkButton *button3, gpointer user_data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open Image", GTK_WINDOW(user_data), GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

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
        int val = gtk_range_get_value(GTK_RANGE(slider));
        GdkPixbuf *saturated = gdk_pixbuf_copy(imidz_pojntr);
        gdk_pixbuf_saturate_and_pixelate(imidz_pojntr, saturated, 0.0, FALSE);
        guchar *p = gdk_pixbuf_get_pixels(saturated);
        int size = gdk_pixbuf_get_height(saturated) * gdk_pixbuf_get_rowstride(saturated);
        for (int i = 0; i < size; i++) {
            p[i] = (p[i] > val) ? 255 : 0;
        }
        imidztukonvert = gtk_image_new_from_pixbuf(saturated);
        gtk_box_pack_start(GTK_BOX(box1), imidztukonvert, TRUE, TRUE, 50);
        gtk_widget_show_all(window);
    }
}

void save(GtkButton *button4, gpointer user_data) {
    if (imidztukonvert) {
        GError *err = NULL;
        GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(imidztukonvert));
        gdk_pixbuf_save(pixbuf, "out.jpg", "jpeg", &err, "quality", "95", NULL);
    }
}

void export(GtkButton *button5, gpointer user_data) {
    if (!imidztukonvert) return;
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(imidztukonvert));
    if (!pixbuf) return;
    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);

    if (width == height) {
        guchar *p = gdk_pixbuf_get_pixels(pixbuf);
        char line[32];
        scriptfile = fopen("script.txt", "a");
        if (scriptfile == NULL) {
            g_print("Error: Could not open script.txt for writing.\n");
            return;
        }
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                guchar *pixel = p + y * rowstride + x * n_channels;
                if (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0) {
                        fprintf(scriptfile, "x %d y %d\n", x, y);
                }
            }
        }       
    }
    fprintf(scriptfile, "EOF");
    fclose(scriptfile);
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
    GtkWidget *button4;
    GtkWidget *button5;
    GtkWidget *box;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    button1 = gtk_button_new_with_label("Convert!");
    button2 = gtk_button_new_with_label("Exit");
    button3 = gtk_button_new_with_label("Open File");
    button4 = gtk_button_new_with_label("Save image");
    button5 = gtk_button_new_with_label("Export file for NXT");
    slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 255, 1);
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    imidztukonvert = gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_file("./img/placeholder.png", &err));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(button1, "clicked", G_CALLBACK(convert), NULL);
    g_signal_connect(button2, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(button3, "clicked", G_CALLBACK(selectfile), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(save), NULL);
    g_signal_connect(button5, "clicked", G_CALLBACK(export), NULL);
    GdkRGBA black = {0.0, 0.0, 0.0, 1.0};
    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &black);
    gtk_window_set_title(GTK_WINDOW(window), "Konverter obrazku do souradnic pro robota");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    gtk_container_add(GTK_CONTAINER(window), box1);
    gtk_box_pack_start(GTK_BOX(box1), box, TRUE, TRUE, 50);
    gtk_box_pack_start(GTK_BOX(box1), imidztukonvert, TRUE, TRUE, 50);
    gtk_box_pack_start(GTK_BOX(box), button1, TRUE, TRUE, 50);
    gtk_box_pack_start(GTK_BOX(box), button2, TRUE, TRUE, 50);
    gtk_box_pack_start(GTK_BOX(box), button3, TRUE, TRUE, 50);
    gtk_box_pack_start(GTK_BOX(box), button4, TRUE, TRUE, 50);
    gtk_box_pack_start(GTK_BOX(box), button5, TRUE, TRUE, 50);
    gtk_box_pack_start(GTK_BOX(box), slider, TRUE, TRUE, 50);



    
    gtk_widget_show_all(window);

    gtk_main();


};
