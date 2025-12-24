#include <unistd.h>
#include "std/outstr.h" /* Includes std/iobuf.h and std/std.h as well */

#define OUT_FILE_FORMAT	".txt"
#define OUT_FILE_LEN	4

void cfta(char *);

static struct _prog_mode {
	char blockmode	: 1; /* Block mode: separate each byte by a space */
	char is_stdout 	: 1; /* Should output be printed into the standard output? */
} prog_mode = {0};


int main(int argc, char *argv[]) {
	while (--argc && **++argv == '-') switch (*++*argv) {
		case 'b':
			prog_mode.blockmode	= 1;
			break;
		case 's':
			prog_mode.is_stdout	= 1;
			break;
		default:
			outputstr(bstderr, "Invalid argument(s) passed.\n");
			return 0;
	}

	if (argc <= 0) {
		outputstr(bstderr, "No files specified.\n"); exit(EXIT_FAILURE);
	}

	while (argc-- > 0) {
		cfta(*argv);
		outputstr(bstdout, "Done with *s\n", *argv);
		argv++;
	}
}

void cfta(char *fname) {
	char outfname[cstrlen(fname) + OUT_FILE_LEN + 1];
	IOBUF *inbp, *outbp;
	int c, i;


	if ((inbp = bopen(fname, IOBM_RO)) == NULL) {
		outputstr(bstderr, "Can't open *s; did you get the name right?\n", fname); exit(EXIT_FAILURE);
	}

	if (!prog_mode.is_stdout) {
		cstrcpy(outfname, fname); cstrcat(outfname, OUT_FILE_FORMAT);
		if ((outbp = bopen(outfname, IOBM_WO)) == NULL) {
			outputstr(bstderr, "Can't open *s for writing. Maybe it already exists?\n", outfname); exit(EXIT_FAILURE);
		}
	} else {
		outputstr(bstdout, "\n*s in binary:\n", fname);
		outbp = bstdout;
	}

	while ((c = readc(inbp)) != EOF) {
		for (i = 7; i >= 0; i--)
			writec(outbp, ((c >> i) & 1) + '0');
		if (prog_mode.blockmode) writec(outbp, ' ');
	}

	bclose(inbp);
	if (!prog_mode.is_stdout) bclose(outbp);
	else {
		writec(outbp, '\n');
		_flushbuf(outbp);
	}
}
