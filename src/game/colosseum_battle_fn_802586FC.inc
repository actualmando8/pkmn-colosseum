    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r7, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    mr r30, r6
    bl fn_8025C264
    mr r31, r3
    mr r3, r29
    mr r4, r30
    li r5, 0x1c
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80258744
    li r3, 0x0
    b @L_802587A4
@L_80258744:
    mr r3, r29
    mr r4, r30
    li r5, 0xc
    bl fn_80237DBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80258768
    li r3, 0x0
    b @L_802587A4
@L_80258768:
    mr r3, r29
    mr r4, r30
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025878C
    li r3, 0x0
    b @L_802587A4
@L_8025878C:
    cmpwi r31, 0x0
    bne @L_8025879C
    li r3, 0x0
    b @L_802587A4
@L_8025879C:
    cmpwi r31, -0x1
    li r3, 0x1
@L_802587A4:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
