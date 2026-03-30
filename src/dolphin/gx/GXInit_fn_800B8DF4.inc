    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    bl OSDisableInterrupts
    li r0, 0x61
    lis r5, 0xcc01
    lis r4, 0x4500
    stb r0, -0x8000(r5)
    addi r0, r4, 0x2
    stw r0, -0x8000(r5)
    mr r31, r3
    bl fn_800B8920
    li r0, 0x0
    stb r0, -24152(r13)
    mr r3, r31
    bl OSRestoreInterrupts
    bl OSDisableInterrupts
    mr r31, r3
    b @L_800B8E4C
@L_800B8E44:
    addi r3, r13, -24148
    bl fn_800A238C
@L_800B8E4C:
    lbz r0, -24152(r13)
    cmplwi r0, 0x0
    beq @L_800B8E44
    mr r3, r31
    bl OSRestoreInterrupts
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
