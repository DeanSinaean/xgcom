/*
 * Xcomtool (Xiyou Linux Group)
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the
 * terms of the GNU General Public License.
 *
 * file name: support.c
 * modify date: 2009-7-30
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gtk/gtk.h>

#include "support.h"

static GList *pixmaps_directories = NULL;

/* Use this function to set the directory containing installed pixmaps. */
void
add_pixmap_directory (const gchar *directory)
{
	pixmaps_directories = g_list_prepend (pixmaps_directories,
                                        g_strdup (directory));
}

/* This is an internally used function to find pixmap files. */
static gchar*
find_pixmap_file (const gchar *filename)
{
	GList *elem;

	/* We step through each of the pixmaps directory to find it. */
	elem = pixmaps_directories;
	while (elem)
	{
		gchar *pathname = g_strdup_printf ("%s%s%s", (gchar*)elem->data,
						 G_DIR_SEPARATOR_S, filename);
		if (g_file_test (pathname, G_FILE_TEST_EXISTS))
			return pathname;
		g_free (pathname);
		elem = elem->next;
	}
	return NULL;
}

/* This is an internally used function to create pixmaps. */
GtkWidget*
create_pixmap (GtkWidget *widget, const gchar *filename)
{
	gchar *pathname = NULL;
	GtkWidget *pixmap;

	if (!filename || !filename[0])
		return gtk_image_new ();

	pathname = find_pixmap_file (filename);

	if (!pathname)	{
		g_warning (_("Couldn\47t find pixmap file\72 \45s"), filename);
		return gtk_image_new ();
	}

	pixmap = gtk_image_new_from_file (pathname);
	g_free (pathname);
	return pixmap;
}

/* This is an internally used function to create pixmaps. */
GdkPixbuf*
create_pixbuf (const gchar *filename)
{
	gchar *pathname = NULL;
	GdkPixbuf *pixbuf;
	GError *error = NULL;

	if (!filename || !filename[0])
		return NULL;

	pathname = find_pixmap_file (filename);

	if (!pathname)	{
		g_warning (_("Couldn\47t find pixmap file\72 \45s"), filename);
		return NULL;
	}

	pixbuf = gdk_pixbuf_new_from_file (pathname, &error);
	if (!pixbuf) {
		fprintf (stderr, "Failed to load pixbuf file: %s: %s\n",
		pathname, error->message);
		g_error_free (error);
	}
	g_free (pathname);
	return pixbuf;
}
