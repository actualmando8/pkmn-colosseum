    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    li r30, 0x0
    mr r31, r3
    li r29, 0x0
@L_8025D8B8:
    mr r3, r31
    bl fn_8006B09C
    clrlwi r4, r29, 16
    addi r3, r3, 0xb44
    bl fn_8012AC08
    bl fn_80123FBC
    clrlwi r3, r3, 24
    subic r0, r3, 0x1
    subfe r0, r0, r3
    clrlwi. r0, r0, 24
    beq @L_8025D8F0
    clrlwi r3, r30, 16
    addi r0, r3, 0x1
    clrlwi r30, r0, 16
@L_8025D8F0:
    addi r29, r29, 0x1
    cmpwi r29, 0x6
    blt @L_8025D8B8
    mr r3, r30
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
