    nofralloc
    mflr r0
    li r3, 0x0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    lwz r0, -24044(r13)
    stb r3, -24036(r13)
    cmpwi r0, 0x0
    bne @L_800CEB4C
    bl OSDisableInterrupts
    addi r31, r3, 0x0
    addi r3, r1, 0x8
    bl fn_800CECAC
    lwz r0, 0x8(r1)
    clrlwi. r0, r0, 31
    beq @L_800CEB44
    addi r3, r1, 0x8
    bl fn_800CEF10
    lwz r0, 0x8(r1)
    clrlwi r0, r0, 3
    stw r0, 0x8(r1)
    lwz r4, 0x8(r1)
    rlwinm r3, r4, 0, 3, 7
    subis r0, r3, 0x1f00
    cmplwi r0, 0x0
    bne @L_800CEB44
    clrlwi r3, r4, 17
    stw r4, -24048(r13)
    li r0, 0x1
    stw r3, -24044(r13)
    stb r0, -24036(r13)
@L_800CEB44:
    mr r3, r31
    bl OSRestoreInterrupts
@L_800CEB4C:
    lwz r3, -24044(r13)
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
