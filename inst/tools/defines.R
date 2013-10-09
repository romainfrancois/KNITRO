knitro_h <- file.path( Sys.getenv( "KNITRO" ), "include", "knitro.h" )
defines <- readLines( knitro_h )
rx <- "^# *define * (KTR_[^ ]+) *([^ ]+).*$"
defines <- grep( rx, defines, value = TRUE )
name <- gsub( rx, "\\1", defines )
value <- gsub( rx, "\\2", defines )
numerics <- grepl( "[.e]", value)
value <- ifelse( numerics, value, paste( value, "L", sep = "" ) )

output <- file( "R/defines.R", open  = "w" )
writeLines( sprintf( "%s <- %s", name, value ), output )
close(output)

