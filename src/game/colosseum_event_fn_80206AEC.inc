    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr. r29, r3
    mr r30, r4
    mr r31, r5
    beq @L_80206C28
    cmplwi r30, 0x0
    beq @L_80206C28
    cmplwi r29, 0x0
    beq @L_80206BDC
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    bl fn_80124A60
    mr r3, r29
    li r4, 0x0
    li r5, 0xcd
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x1
    bl fn_8011B950
    mr r3, r29
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    li r7, -0x1
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0xcf
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0xd0
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0xd1
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0xd2
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
@L_80206BDC:
    mr r3, r29
    mr r7, r30
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r4, r30
    bl fn_8011F5FC
    mr r3, r29
    extsh r7, r31
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_801254B4
@L_80206C28:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
