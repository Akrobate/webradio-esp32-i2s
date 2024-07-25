DIST_DIR="dist"
ESP_DATA_DIR="../WebRadio/data/webinterface/"


if [ -d "$DIST_DIR" ]; then
    rm -rf "$DIST_DIR"
fi

mkdir -p "$DIST_DIR"

cp index.html dist/index.html
cp style.css dist/style.css
cp repository.js dist/repository.js
cp index.js dist/index.js

SEARCH_STRING='<script src="mock-server.js"></script>'
REPLACE_STRING='<script src="repository.js"></script>'

ESCAPED_SEARCH_STRING=$(printf '%s\n' "$SEARCH_STRING" | sed -e 's/[\/&]/\\&/g' -e 's/"/\\\"/g')
ESCAPED_REPLACE_STRING=$(printf '%s\n' "$REPLACE_STRING" | sed -e 's/[\/&]/\\&/g' -e 's/"/\\\"/g')

sed -i "s/$ESCAPED_SEARCH_STRING/$ESCAPED_REPLACE_STRING/g" $DIST_DIR/index.html

cp $DIST_DIR/* $ESP_DATA_DIR

