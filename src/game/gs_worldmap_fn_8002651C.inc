    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    lwz r3, 0x60(r3)
    li r30, 0x0
    lwz r0, 0x1c(r3)
    cmpwi r0, 0x2
    beq @L_8002655C
    li r0, 0x0
    li r3, 0x0
    stb r0, 0x67(r29)
    b @L_800265A4
    @L_8002655C:
    lwz r0, 0x20(r3)
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r31, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002659C
    mr r3, r31
    bl fn_801231A4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8002659C
    li r30, 0xff
    @L_8002659C:
    stb r30, 0x67(r29)
    li r3, 0x0
    @L_800265A4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
