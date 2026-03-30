    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_802079C8
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    bl fn_80124A60
    mr r3, r31
    li r4, 0x0
    li r5, 0xcd
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x1
    bl fn_8011B950
    mr r3, r31
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    li r7, -0x1
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xcf
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xd0
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xd1
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xd2
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
@L_802079C8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
