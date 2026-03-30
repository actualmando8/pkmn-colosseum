    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    addi r30, r4, 0x0
    stw r29, 0x14(r1)
    addi r29, r3, 0x0
    bl OSDisableInterrupts
    lwz r0, -24048(r13)
    addi r31, r3, 0x0
    rlwinm. r0, r0, 0, 15, 15
    beq @L_800CEA78
    li r3, 0x1000
    b @L_800CEA7C
@L_800CEA78:
    li r3, 0x0
@L_800CEA7C:
    addis r3, r3, 0x2
    addi r0, r30, 0x3
    addi r4, r29, 0x0
    clrrwi r5, r0, 2
    subi r3, r3, 0x2000
    bl fn_800CEE34
    li r0, 0x0
    stw r0, -24044(r13)
    mr r3, r31
    stb r0, -24036(r13)
    bl OSRestoreInterrupts
    lwz r0, 0x24(r1)
    li r3, 0x0
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
