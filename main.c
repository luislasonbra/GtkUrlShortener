#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <curl/curl.h>

#include "cJSON/cJSON.h"

size_t ExtractShortUrl(void *buffer,size_t size,size_t realsize,void *userbuf)
{
    cJSON *my_json = cJSON_Parse((char *)buffer);
    GtkWidget *window = userbuf,*dialog;
    my_json = my_json->child;
    while(strcmp("id",my_json->string) != 0)
    {
        my_json = my_json->next;
    }
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,"Short URL: %s",my_json->valuestring);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return 0;
}

static void MakeShortURL(GtkWidget *parent,gpointer data)
{
    void **_data = data;
    GtkWidget *window = _data[0],*entry = _data[1];
    GtkWidget *dialog;
    CURL *curl = _data[2];
    gchar *longurl = gtk_editable_get_chars(GTK_EDITABLE(entry),0,-1);
    if(strlen(longurl) > 0)
    {
        char *json_post = malloc(strlen(longurl) + 16);
        sprintf(json_post,"{\"longUrl\": \"%s\"}",longurl);
        struct curl_slist *header = NULL;
        header = curl_slist_append(header,"Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_post);
		curl_easy_setopt(curl, CURLOPT_URL, "https://www.googleapis.com/urlshortener/v1/url");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ExtractShortUrl);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, window);
		curl_easy_perform(curl);
		free(json_post);
    }
    else
    {
        dialog = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"Error! Insert a valid URL!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

int main(int argc,char **argv)
{
    GtkWidget *window,*vbox,*hbox,*label,*button,*entry;
    void *data[3];
    CURL *curl = curl_easy_init();
    gtk_init(&argc,&argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"GtkUrlShortener");
    gtk_window_set_default_size(GTK_WINDOW(window),300,100);
    gtk_container_set_border_width(GTK_CONTAINER(window),5);
    vbox = gtk_vbox_new(TRUE,2);
    hbox = gtk_hbox_new(FALSE,2);
    gtk_container_add(GTK_CONTAINER(window),vbox);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,TRUE,2);
    label = gtk_label_new("Insert long URL");
    gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,2);
    entry = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(entry),TRUE);
    gtk_box_pack_start(GTK_BOX(hbox),entry,TRUE,TRUE,2);
    hbox = gtk_hbox_new(TRUE,2);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,TRUE,2);
    button = gtk_button_new_with_label("Make short URL!");
    gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,2);
    data[0] = window;
    data[1] = entry;
    data[2] = curl;
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(MakeShortURL),data);
    gtk_widget_show_all(window);
    gtk_main();
    curl_easy_cleanup(curl);
    return 0;
}
