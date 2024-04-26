#ifndef MENU_ITEM_DISPLAY_H
#define MENU_ITEM_DISPLAY_H

class menu_item_display : public menu_item {
    private:
        fptr display, lbtn, rbtn;
    public:
        menu_item_display(const char* key, fptr display, fptr lbtn, fptr rbtn) : menu_item(key, MENU_ITEM_DISPLAY) {
            this->display = display;
            this->lbtn = lbtn;
            this->rbtn = rbtn;
        }

        fptr get_display() override {
            return this->display;
        }
        fptr get_lbtn() override { return this->lbtn; }
        fptr get_rbtn() override { return this->rbtn; }
};

#endif
