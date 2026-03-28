    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    bl fn_8025D808
    clrlwi r29, r3, 16
    li r31, 0x0
    b @L_80062308
@L_800622B4:
    mr r3, r28
    mr r4, r31
    bl fn_8025D938
    mr r30, r3
    cmplwi r30, 0x0
    beq @L_80062304
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80062304
    mr r3, r30
    li r4, 0x0
    li r5, 0x7b
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80062304
    li r3, 0x0
    b @L_80062314
@L_80062304:
    addi r31, r31, 0x1
@L_80062308:
    cmpw r31, r29
    blt @L_800622B4
    li r3, 0x1
@L_80062314:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
