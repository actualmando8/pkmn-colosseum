    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r4, 0x8001
    clrlwi r3, r3, 24
    addi r5, r4, -0x7b40
    addi r4, r1, 0x8
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80008494
    li r3, -0x1
    b @L_800084B0
    @L_80008494:
    lwz r0, 0x8(r1)
    cmplwi r0, 0xb
    ble @L_800084A8
    li r0, 0xb
    stw r0, 0x8(r1)
    @L_800084A8:
    bl fn_8001E200
    lwz r3, 0x8(r1)
    @L_800084B0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
