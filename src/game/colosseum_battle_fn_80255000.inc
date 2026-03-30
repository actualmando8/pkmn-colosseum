    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r28, r3
    mr r30, r4
    mr r29, r6
    bl fn_802383A4
    mr r0, r3
    mr r3, r28
    mr r31, r0
    mr r4, r29
    bl fn_802383A4
    mr r0, r3
    mr r3, r30
    mr r30, r0
    bl fn_80216048
    clrlwi. r0, r3, 24
    bne @L_80255054
    li r3, 0x0
    b @L_802550DC
@L_80255054:
    clrlwi. r0, r31, 16
    bne @L_80255064
    clrlwi. r0, r30, 16
    beq @L_802550AC
@L_80255064:
    clrlwi r3, r31, 16
    cmplwi r3, 0xaf
    beq @L_802550AC
    clrlwi r0, r30, 16
    cmplwi r0, 0xaf
    beq @L_802550AC
    cmplwi r3, 0x0
    beq @L_80255094
    mr r3, r31
    bl fn_80142984
    clrlwi. r0, r3, 24
    beq @L_802550AC
@L_80255094:
    clrlwi. r0, r30, 16
    beq @L_802550B4
    mr r3, r30
    bl fn_80142984
    clrlwi. r0, r3, 24
    bne @L_802550B4
@L_802550AC:
    li r3, 0x0
    b @L_802550DC
@L_802550B4:
    mr r3, r28
    mr r4, r29
    li r5, 0x3c
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802550D8
    li r3, 0x0
    b @L_802550DC
@L_802550D8:
    li r3, 0x1
@L_802550DC:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
