    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r28, r3
    bl fn_8006B1D4
    li r30, 0x0
    mr r29, r28
    mr r31, r30
    clrlwi r28, r3, 16
@L_8025D830:
    mr r3, r29
    bl fn_8006B09C
    clrlwi r4, r31, 16
    addi r3, r3, 0xb44
    bl fn_8012AC08
    bl fn_80123FBC
    clrlwi r3, r3, 24
    subic r0, r3, 0x1
    subfe r0, r0, r3
    clrlwi. r0, r0, 24
    beq @L_8025D868
    clrlwi r3, r30, 16
    addi r0, r3, 0x1
    clrlwi r30, r0, 16
@L_8025D868:
    addi r31, r31, 0x1
    cmpwi r31, 0x6
    blt @L_8025D830
    clrlwi r0, r30, 16
    mr r3, r28
    cmplw r0, r28
    bge @L_8025D888
    mr r3, r30
@L_8025D888:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
