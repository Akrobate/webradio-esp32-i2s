DIST_DIR="dist"

if [ -d "$DIST_DIR" ]; then
    rm -rf "$DIST_DIR"
fi

mkdir -p "$DIST_DIR"

if [ ! -d "node_modules" ]; then
    npm install
fi

cp index.html dist/index.html

npx minify index.js > dist/index.min.js
npx minify style.css > dist/style.min.css
npx minify mock-server.js > dist/mock-server.min.js

replace_in_index() {
    TAG=$1
    SEARCH_STRING=$2
    REPLACE_STRING=$(<"$3")

    ESCAPED_SEARCH_STRING=$(printf '%s\n' "$SEARCH_STRING" | sed -e 's/[\/&]/\\&/g' -e 's/"/\\\"/g')
    ESCAPED_REPLACE_STRING=$(printf '%s\n' "$REPLACE_STRING" | sed -e 's/[\/&]/\\&/g' -e 's/"/\\\"/g')

    sed -i "s/$ESCAPED_SEARCH_STRING/<$TAG>$ESCAPED_REPLACE_STRING<\/$TAG>/g" $DIST_DIR/index.html
}


replace_in_index "script" '<script src="mock-server.js"></script>' "$DIST_DIR/mock-server.min.js"
replace_in_index "script" '<script src="index.js"></script>' "$DIST_DIR/index.min.js"
replace_in_index "style" '<link rel="stylesheet" href="style.css">' "$DIST_DIR/style.min.css"


npx minify $DIST_DIR/index.html > $DIST_DIR/index.min.html

# Clean workspace
#rm -rf $DIST_DIR/*.js $DIST_DIR/*.css
mv $DIST_DIR/index.min.html $DIST_DIR/index.html




