mURIel
======

A C++ library to parse, validate, modify, and factorize Uniform Resource Identifiers (i.e. URLs)

mURIeL - I have not figured out yet what it means, but what about "manipulation of URI elements with Logic"

I havent' had that much time to care about mURIeL and I have many mock-ups to her, so dont mind looking into there
and adding/improving stuff if you wish. For now it permits to parse a URL, and individually access/modify the components,
as well as appending/inserting elements, as path or port. Also you can access individual parts of the path easily, like the directory
or just the filename, if existing. Moreover it encapsulates a datamodel called ByteRange that holds all the information to a an HTTP-range-request
and has the logic to parse a range from a string in the usual format. Inversely the range can be output in this way or in any given "printf-pattern".
You can also attach listeners to the Url to propagate modifications of the data model.

