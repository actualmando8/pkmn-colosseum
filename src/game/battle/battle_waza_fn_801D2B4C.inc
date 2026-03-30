    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    bl fn_800E0BE4
    rlwinm. r0, r30, 0, 24, 31
    lfs f2, -21704(r2)
    beq @L_801D2BB4
    rlwinm. r0, r31, 0, 26, 26
    lis r3, -32713
    li r4, 0x8
    addi r3, r3, 8032
    beq @L_801D2B94
    addi r3, r3, 0x130
    b @L_801D2C54
@L_801D2B94:
    rlwinm. r0, r31, 0, 25, 25
    beq @L_801D2BA4
    addi r3, r3, 0x17c
    b @L_801D2C54
@L_801D2BA4:
    rlwinm. r0, r31, 0, 24, 24
    beq @L_801D2BF0
    addi r3, r3, 0x1c8
    b @L_801D2C54
@L_801D2BB4:
    rlwinm. r0, r31, 0, 26, 26
    lis r3, -32713
    li r4, 0xd
    addi r3, r3, 8640
    beq @L_801D2BD0
    addi r3, r3, 0x2ac
    b @L_801D2C54
@L_801D2BD0:
    rlwinm. r0, r31, 0, 25, 25
    beq @L_801D2BE0
    addi r3, r3, 0x2f8
    b @L_801D2C54
@L_801D2BE0:
    rlwinm. r0, r31, 0, 24, 24
    beq @L_801D2BF0
    addi r3, r3, 0x344
    b @L_801D2C54
@L_801D2BF0:
    mtctr r4
    cmpwi r4, 0x0
    ble @L_801D2C18
@L_801D2BFC:
    lfs f0, 0x0(r3)
    fadds f2, f2, f0
    fcmpo cr0, f1, f2
    bge @L_801D2C10
    b @L_801D2C54
@L_801D2C10:
    addi r3, r3, 0x4c
    bdnz @L_801D2BFC
@L_801D2C18:
    rlwinm. r0, r30, 0, 24, 31
    beq @L_801D2C3C
    li r3, 0x8
    bl fn_800E0C04
    mulli r4, r3, 0x4c
    lis r3, -32713
    addi r0, r3, 8032
    add r3, r0, r4
    b @L_801D2C54
@L_801D2C3C:
    li r3, 0xd
    bl fn_800E0C04
    mulli r4, r3, 0x4c
    lis r3, -32713
    addi r0, r3, 8640
    add r3, r0, r4
@L_801D2C54:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
