cmpwi r3, 0x0
blt @80077D9C
lwz r0, lbl_80478928@sda21(r0)
cmplw r0, r3
bgt @80077DA4
@80077D9C
li r3, 0x0
blr
@80077DA4
lis r4, lbl_802EE458@ha
slwi r0, r3, 1
addi r3, r4, lbl_802EE458@l
lhzx r3, r3, r0
blr
