/*----------------------------FOND € Font Family Description----------------------------*/
/* Note: this FOND resource definition only works when the tables at the end of the
		 resource are in this order:	1 - Family Character-Width Table
		 								2 - Style Mapping Table
										3 - Kerning Tables
*/
type 'FOND' {
		/* Flags Word */
		boolean		proportionalFont, fixedWidthFont;
		boolean		useFractWidthTable, dontUseFractWidthTable;
		boolean		computeFixedPointExtra, useIntegerExtra;
		boolean		useFractEnable, ignoreFractEnable;
		boolean		canAdjustCharSpacing, dontAdjustCharSpacing;
		unsigned hex bitstring[9] = 0;							/* Reserved				*/
		boolean		noCharWidthTable, hasCharWidthTable;
		boolean		noImageHeightTable, hasImageHeightTable;

		integer;												/* Family ID number		*/
	First:
		integer;												/* first char			*/
	Last:
		integer;												/* last char			*/
		integer;												/* Ascent				*/
		integer;												/* Descent				*/
		integer;												/* Leading				*/
		integer;												/* Width Max			*/
	WidthOffset:
		unsigned hex longint = WidthTable[1] >> 3;				/* Width table offset	*/
	KerningOffset:
		unsigned hex longint = KerningTable[1] >> 3;			/* Kerning table offset	*/
	StyleMapOffset:
		unsigned hex longint = StyleTable[1] >> 3;				/* Style map offset		*/
		integer;												/* reserved				*/
		integer;												/* ex wid bold			*/
		integer;												/* ex wid italic		*/
		integer;												/* ex wid underline		*/
		integer;												/* ex wid outline		*/
		integer;												/* ex wid shadow		*/
		integer;												/* ex wid condensed		*/
		integer;												/* ex wid extended		*/
		integer;												/* reserved				*/
		longint;												/* reserved for intl	*/
	Version:
		integer;												/* version				*/
	
		/* Font Association Table */
		integer = $$CountOf(FontEntries)-1; 					/* # of font entries	*/
		wide array FontEntries {
			integer;											/* Font size			*/
			integer;											/* Font style			*/
			integer;											/* Resource ID of FONT	*/
		};
		/*  */
		array [$$Word(Version) == 2] {
	OffsetTableStart:
			integer = $$CountOf(OffsetTable) - 1;
			array OffsetTable {
				longint = (BBoxStart[1] - OffsetTableStart[1]) >> 3;				
			};
			/* Font Bounding Box Table */
	BBoxStart:
			integer = $$CountOf(BBoxTable) - 1;
			wide array BBoxTable {
				fill bit[9];									/* Reserved				*/
				Boolean		noExtendedStyle, EXTENDEDstyle;		/* Extended style		*/
				Boolean		noCondensedStyle, CONDENSEDstyle;	/* Condensed style		*/
				Boolean		noShadowStyle, SHADOWstyle;			/* Shadow style			*/
				Boolean		noOutlineStyle, OUTLINEstyle;		/* Outline style		*/
				Boolean		noUnderline, UNDERLINEstyle;		/* Underline style		*/
				Boolean		noItalicStyle, ITALICstyle;			/* Italic style			*/
				Boolean		noBoldStyle, BOLDstyle;				/* Bold style			*/
				Rect;
			};
		};

		/* Family Character-Width Table */
		/* This outer array below handles the case when the width table offset (WidthOffset:)
		   is zero. */
		array [$$Long(WidthOffset) != 0] {
	WidthTable:
			integer = $$CountOf(WidthTable) - 1;				/* # of width tables	*/
			wide array WidthTable {
				fill bit[9];									/* Reserved				*/
				Boolean		noExtendedStyle, EXTENDEDstyle;		/* Extended style		*/
				Boolean		noCondensedStyle, CONDENSEDstyle;	/* Condensed style		*/
				Boolean		noShadowStyle, SHADOWstyle;			/* Shadow style			*/
				Boolean		noOutlineStyle, OUTLINEstyle;		/* Outline style		*/
				Boolean		noUnderline, UNDERLINEstyle;		/* Underline style		*/
				Boolean		noItalicStyle, ITALICstyle;			/* Italic style			*/
				Boolean		noBoldStyle, BOLDstyle;				/* Bold style			*/
				
				wide array [$$Word(Last) - $$Word(First) + 3] {
					unsigned hex integer;						/* Width of character	*/
				};
			};
		};
	
		/* Style Mapping Table */
		/* This outer array below handles the case when the width table offset (WidthOffset:)
		   is zero. */
		array [$$Long(StyleMapOffset) != 0] {
	StyleTable:
			unsigned hex integer;								/* Font class			*/
	CharCodeOffset:
			unsigned hex longint =								/* Encoding table offset*/
				(CharCodeTable[1,1] - StyleTable[1]) / 8 * (CharCodeTable[1,1] != 0);
			fill long;											/* Reserved				*/
			array [48] {
				byte;
			};
			/* Style Name Table */
			integer = $$CountOf(StyleNames);					/* Number of strings	*/
			pstring;											/* Full base font name	*/
			array StyleNames {
				pstring;										/* Style suffix names	*/
			};
			
			/* Character Set Encoding Table */
			/* This outer array below handles the case when the character set encoding
			   offset is zero (CharCodeOffset:) */
			array [$$Long(CharCodeOffset[1]) != 0] {
	CharCodeTable:
				integer = $$CountOf(CharacterCodes);			/* Number of entries	*/
				wide array CharacterCodes {
					char;										/* Character code		*/
					pstring;									/* Char name string		*/
				};
			};
		};

		/* Kerning Tables */
		/* This outer array below handles the case when the kerning table offset
		   (KerningOffset:) is zero. */
		array [$$Long(KerningOffset) != 0] {
			/* Kerning Tables */
	KerningTable:
			integer = $$CountOf(KerningTables) - 1;				/* Number of tables		*/
			wide array KerningTables {
				fill bit[9];									/* Reserved				*/
				Boolean		noExtendedStyle, EXTENDEDstyle;		/* Extended style		*/
				Boolean		noCondensedStyle, CONDENSEDstyle;	/* Condensed style		*/
				Boolean		noShadowStyle, SHADOWstyle;			/* Shadow style			*/
				Boolean		noOutlineStyle, OUTLINEstyle;		/* Outline style		*/
				Boolean		noUnderline, UNDERLINEstyle;		/* Underline style		*/
				Boolean		noItalicStyle, ITALICstyle;			/* Italic style			*/
				Boolean		noBoldStyle, BOLDstyle;				/* Bold style			*/
				integer = $$CountOf(KerningTableEntry);			/* # of entries			*/
				wide array KerningTableEntry {
					char;										/* first char of pair	*/
					char;										/* second char of pair	*/
					unsigned hex integer;						/* kerning offset		*/
				};
			};
		};
};
/*----------------------------FONT € Font Description-----------------------------------*/
/* PROBLEMS: the offset to the offset/width table has been changed to a longint, with the
			 high word stored in the neg descent field (if its not -1).  Rez can't handle
			 this. */
type 'FONT' {	
		/* Font Type Flags */
		boolean = 1;											/* Reserved				*/
		boolean		doExpandFont, dontExpandFont;
		boolean		proportionalFont, fixedWidthFont;
		boolean = 1;											/* Reserved				*/
		unsigned bitstring[2] = 0;								/* Reserved				*/
		boolean		blackFont, colorFont;
		boolean		notSynthetic, isSynthetic;
		boolean		nofctbRsrc, hasfctbRsrc;
		unsigned bitstring[3] = 0;								/* Reserved				*/
		unsigned bitstring[2]	oneBit, twoBit,					/* Font depth			*/
								fourBit, eightBit;
		boolean		noCharWidthTable, hasCharWidthTable;
		boolean		noImageHeightTable, hasImageHeightTable;

	FirstChar:
		integer;												/* first char			*/
	LastChar:
		integer;												/* last char			*/
		integer;												/* width max			*/
		integer;												/* kern max				*/
		integer;												/* neg descent			*/
		integer;												/* font rect width		*/
	Height:
		integer;												/* font rect height		*/
	Offset:
		unsigned integer = ((WidthTable-Offset)/16);			/* offset to off/wid tab*/		
		integer;												/* ascent				*/
		integer;												/* descent				*/
		integer;												/* leading				*/
	RowWords:
		integer;												/* row width (in words)	*/
		
		/* Tables */
		/* Bit image */
		hex string [$$Word(RowWords) * $$Word(Height) * 2];

		/* Location Table */
		array [$$Word(LastChar) - $$Word(FirstChar) + 3] {
			integer;
		};
		
		/* Offset/Width Table */
	WidthTable:
		array [$$Word(LastChar) - $$Word(FirstChar) + 3] {
			integer;
		};

		/* Don't know what this table does, but it needs to be there - Table */
	UnknownTable:
		array [$$Word(LastChar) - $$Word(FirstChar) + 3] {
			integer;
		};
};
/*----------------------------NFNT € Font Description-----------------------------------*/
type 'NFNT' as 'FONT';
