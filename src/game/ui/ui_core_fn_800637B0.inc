    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    bl fn_8025DA88
    mr r30, r3
    bl fn_8025DAAC
    mr r0, r3
    mr r4, r30
    mr r31, r0
    bl fn_8006B1F4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800637F8
    mr r3, r31
    mr r4, r30
    bl fn_8006B2A4
@L_800637F8:
    li r3, 0x3
    bl fn_8006B3C8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80063874
    li r30, 0x1
    li r31, 0x0
@L_80063814:
    mr r3, r31
    li r4, 0x0
    bl fn_8006B1F4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80063834
    li r30, 0x0
    b @L_80063860
@L_80063834:
    mr r3, r31
    li r4, 0x1
    bl fn_8006B1F4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80063854
    li r30, 0x0
    b @L_80063860
@L_80063854:
    addi r31, r31, 0x1
    cmpwi r31, 0x2
    ble @L_80063814
@L_80063860:
    clrlwi r0, r30, 24
    cmplwi r0, 0x1
    bne @L_80063874
    li r3, 0x3
    bl fn_8006B354
@L_80063874:
    li r3, 0x5
    bl fn_8006B3C8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800638DC
    li r31, 0x1
    li r3, 0x4
    li r4, 0x0
    bl fn_8006B1F4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800638AC
    li r31, 0x0
    b @L_800638C8
@L_800638AC:
    li r3, 0x4
    li r4, 0x1
    bl fn_8006B1F4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800638C8
    li r31, 0x0
@L_800638C8:
    clrlwi r0, r31, 24
    cmplwi r0, 0x1
    bne @L_800638DC
    li r3, 0x5
    bl fn_8006B354
@L_800638DC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
