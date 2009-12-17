GS RENOVATION framework notes.

On default the old GS is used.

Activate RD_GS_RENOVATION flag in the group\bld.inf if RD_GS_RENOVATION framework is 
to be used. Remember to use abld reallyclean before building the project when 
switching between ond and renoved GS framework or you will get mixed files from
the old and new frameworks:
  1. Define/undefine RD_GS_RENOVATION
  2. abld reallyclean
  3. bldmake bldfiles
  4. abld build


Do not use UIDs used by other plugins even for testing purposes. ECOM framework
seems to have some issues with updating the ECOM plugins implementaions and 
their UIDs.

Implemented:
- Main view.
- Default icons.
- Plugins.
- GS FW for loading the plugins.
- Helper classes.

Functionality not implemented yet:
- Help.
- Btimaps in addition to scalable icons.

Bugs not fixed in GS renovation:
-GSAppsPlugin does not handle "trap" panics.




